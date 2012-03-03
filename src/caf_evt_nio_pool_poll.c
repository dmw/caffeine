/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "caf" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
/*
  Caffeine - C Application Framework
  Copyright (C) 2006 Daniel Molina Wegener <dmw@coder.cl>

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

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"

#define IO_EVENT_USE_POLL
#include "caf/caf_evt_nio.h"
#include "caf/caf_evt_nio_pool.h"


io_evt_pool_poll_t *
io_evt_pool_poll_new (int cnt, int tos, int ton) {
	io_evt_pool_poll_t *r = (io_evt_pool_poll_t *)NULL;
	if (cnt > 0) {
		r = (io_evt_pool_poll_t *)xmalloc (IO_EVENT_DATA_POOL_POLL_SZ);
		if (r != (io_evt_pool_poll_t *)NULL) {
			memset ((void *)r, 0, IO_EVENT_DATA_POOL_POLL_SZ);
			r->poll_count = cnt;
			r->poll = (struct pollfd *)xmalloc (IO_EVENT_DATA_POLLFDS_SZ);
			if (r->poll != (struct pollfd *)NULL) {
				caf_io_evt_pool_reset (r);
				r->timeout.tv_sec = tos;
				r->timeout.tv_nsec = ton;
			} else {
				xfree (r);
				r = (io_evt_pool_poll_t *)NULL;
			}
		}
	}
	return r;
}


int
io_evt_pool_poll_delete (io_evt_pool_poll_t *r) {
	if (r != (io_evt_pool_poll_t *)NULL) {
		if (r->poll != (struct pollfd *)NULL) {
			xfree (r->poll);
		}
		xfree (r);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
io_evt_pool_poll_reset (io_evt_pool_poll_t *e) {
	int i;
	if (e != (io_evt_pool_poll_t *)NULL) {
		if (e->poll != (struct pollfd *)NULL) {
			for (i = 0; i < e->poll_count; i++) {
				e->poll[i].events = 0;
				e->poll[i].revents = 0;
				e->poll[i].fd = -1;
			}
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


int
io_evt_pool_poll_add (int fd, io_evt_pool_poll_t *e, int ef) {
	int i, r = -1;
	if (e != (io_evt_pool_poll_t *)NULL) {
		if (e->poll != (struct pollfd *)NULL) {
			for (i = 0; i < e->poll_count; i++) {
				if (e->poll[i].fd == -1) {
					r = i;
					i = e->poll_count;
				}
			}
			if (r >= 0) {
				e->poll[r].fd = fd;
				e->poll[r].events = ef;
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


int
io_evt_pool_poll_hasevent (int fd, io_evt_pool_poll_t *e, int ef) {
	int i;
	if (e != (io_evt_pool_poll_t *)NULL) {
		if (e->poll != (struct pollfd *)NULL) {
			for (i = 0; i < e->poll_count; i++) {
				if (fd == e->poll[i].fd) {
					return (e->poll[i].revents & ef) ? CAF_OK : CAF_ERROR;
				}
			}
		}
	}
	return CAF_ERROR;
}


int
io_evt_pool_poll_getevent (int fd, io_evt_pool_poll_t *e) {
	int i;
	if (e != (io_evt_pool_poll_t *)NULL) {
		if (e->poll != (struct pollfd *)NULL) {
			for (i = 0; i < e->poll_count; i++) {
				if (fd == e->poll[i].fd) {
					return e->poll[i].revents;
				}
			}
		}
	}
	return CAF_ERROR;
}


int
io_evt_pool_poll_etype (int fd, io_evt_pool_poll_t *e) {
	int i, r = 0;
	int wre = POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI;
	int wwe = POLLOUT | POLLWRNORM | POLLWRBAND;
	if (e != (io_evt_pool_poll_t *)NULL) {
		if (e->poll != (struct pollfd *)NULL) {
			for (i = 0; i < e->poll_count; i++) {
				if (fd == e->poll[i].fd) {
					r |= (e->poll[i].revents & wre) ? EVT_IO_READ : 0;
					r |= (e->poll[i].revents & wwe) ? EVT_IO_WRITE : 0;
				}
			}
		}
	}
	return r;
}


int
io_evt_pool_poll_handle (io_evt_pool_poll_t *e) {
	int n = 0;
	if (e != (io_evt_pool_poll_t *)NULL) {
		if (e->poll != (struct pollfd *)NULL) {
			n = poll (e->poll, (nfds_t)e->poll_count, e->timeout.tv_sec);
			return (n > 0) ? CAF_OK : CAF_ERROR;
		}
	}
	return CAF_ERROR;
}

/* caf_evt_io_pool_poll.c ends here */

