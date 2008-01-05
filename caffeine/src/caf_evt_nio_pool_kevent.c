/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "caf" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
/*
  Caffeine - C Application Framework
  Copyright (C) 2006 Daniel Molina Wegener

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
  MA 02110-1301 USA
*/
#ifndef lint
static char Id[] = "$Id$";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "caf/config.h"
#endif /* !HAVE_CONFIG_H */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"

#define IO_EVENT_USE_KEVENT
#include "caf/caf_evt_nio.h"
#include "caf/caf_evt_nio_pool.h"

#define KEVT_ADD_FILTER                     (EV_ADD|EV_ENABLE)

io_evt_pool_kevent_t *
io_evt_pool_kevent_new (int cnt, int tos, int ton) {
	io_evt_pool_kevent_t *r = (io_evt_pool_kevent_t *)NULL;
	if (cnt > 0) {
		r = (io_evt_pool_kevent_t *)xmalloc (IO_EVENT_DATA_POOL_POLL_SZ);
		if (r != (io_evt_pool_kevent_t *)NULL) {
			memset ((void *)r, 0, IO_EVENT_DATA_POOL_POLL_SZ);
			r->kevent_count = cnt;
			r->kevent_sz = (size_t)cnt * IO_EVENT_DATA_KEVENT_SZ;
			r->kevent_src = (struct kevent *)xmalloc (r->kevent_sz);
			r->kevent_dst = (struct kevent *)xmalloc (r->kevent_sz);
			if (r->kevent_src != (struct kevent *)NULL &&
				r->kevent_dst != (struct kevent *)NULL) {
				io_evt_pool_kevent_reset (r);
				r->kfd = kqueue ();
				if (r->kfd < 0) {
					xfree (r->kevent_src);
					xfree (r->kevent_dst);
					xfree (r);
					r = (io_evt_pool_kevent_t *)NULL;
				} else {
					r->timeout.tv_sec = tos;
					r->timeout.tv_nsec = ton;
				}
			} else {
				xfree (r->kevent_src);
				xfree (r->kevent_dst);
				xfree (r);
				r = (io_evt_pool_kevent_t *)NULL;
			}
		}
	}
	return r;
}


int
io_evt_pool_kevent_delete (io_evt_pool_kevent_t *r) {
	if (r != (io_evt_pool_kevent_t *)NULL) {
		if (r->kfd >= 0) {
			close (r->kfd);
		}
		if (r->kevent_src != (struct kevent *)NULL) {
			xfree (r->kevent_src);
		}
		if (r->kevent_dst != (struct kevent *)NULL) {
			xfree (r->kevent_dst);
		}
		xfree (r);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
io_evt_pool_kevent_reset (io_evt_pool_kevent_t *e) {
	int i;
	if (e != (io_evt_pool_kevent_t *)NULL) {
		if (e->kevent_dst != (struct kevent *)NULL) {
			memset ((void *)e->kevent_dst, 0, e->kevent_sz);
		}
		if (e->kevent_src != (struct kevent *)NULL) {
			memset ((void *)e->kevent_src, 0, e->kevent_sz);
			for (i = 0; i < e->kevent_count; i++) {
				EV_SET(&(e->kevent_src[i]), 0, 0, 0, 0, 0, 0);
			}
		}
	}
	return CAF_ERROR;
}


int
io_evt_pool_kevent_add (int fd, io_evt_pool_kevent_t *e, int ef) {
	int i, r;
	if (e != (io_evt_pool_kevent_t *)NULL) {
		if (e->kevent_src != (struct kevent *)NULL) {
			for (i = 0; i < e->kevent_count; i++) {
				if (e->kevent_src[i].ident == 0) {
					EV_SET(&(e->kevent_src[i]), fd, ef, KEVT_ADD_FILTER, 0, 0,
					       0);
					r = kevent (e->kfd, e->kevent_src, e->kevent_count, NULL,
					            0, NULL);
					return (r < 0) ? CAF_ERROR : CAF_OK;
				}
			}
		}
	}
	return CAF_ERROR;
}


int
io_evt_pool_kevent_hasevent (int fd, io_evt_pool_kevent_t *e, int ef) {
	int i, r;
	if (e != (io_evt_pool_kevent_t *)NULL) {
		if (e->kevent_dst != (struct kevent *)NULL) {
			for (i = 0; i < e->kevent_count; i++) {
				if ((int)e->kevent_dst[i].ident == fd) {
					r = (e->kevent_dst[i].filter & ef) ? CAF_OK : CAF_ERROR;
					return r;
				}
			}
		}
	}
	return CAF_ERROR;
}


int
io_evt_pool_kevent_getevent (int fd, io_evt_pool_kevent_t *e) {
	int i;
	if (e != (io_evt_pool_kevent_t *)NULL) {
		if (e->kevent_dst != (struct kevent *)NULL) {
			for (i = 0; i < e->kevent_count; i++) {
				if ((int)e->kevent_dst[i].ident == fd) {
					return (e->kevent_dst[i].filter);
				}
			}
		}
	}
	return CAF_ERROR;
}


int
io_evt_pool_kevent_etype (int fd, io_evt_pool_kevent_t *e) {
	int i, r = 0;
	if (e != (io_evt_pool_kevent_t *)NULL) {
		if (e->kevent_dst != (struct kevent *)NULL) {
			for (i = 0; i < e->kevent_count; i++) {
				if ((int)e->kevent_dst[i].ident == fd) {
					r |= e->kevent_dst[i].filter & EVFILT_READ ?
						EVT_IO_READ : 0;
					r |= e->kevent_dst[i].filter & EVFILT_WRITE ?
						EVT_IO_WRITE : 0;
					return r;
				}
			}
		}
	}
	return r;
}


int
io_evt_pool_kevent_handle (io_evt_pool_kevent_t *e) {
	int r;
	if (e != (io_evt_pool_kevent_t *)NULL) {
		if (e->kevent_dst != (struct kevent *)NULL) {
			r = kevent (e->kfd, NULL, 0, e->kevent_dst, e->kevent_count,
			            &(e->timeout));
			return (r > 0) ? CAF_OK : CAF_ERROR;
		}
	}
	return CAF_ERROR;
}

/* caf_evt_io_pool_kevent.c ends here */

