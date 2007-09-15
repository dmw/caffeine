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

#define IO_EVENT_USE_INOTIFY
#include "caf/caf_evt_fio.h"

static int local_caf_inotify_fd = -1;


fio_evt_t *
caf_fio_evt_new  (caf_io_file_t *f, int type, int to) {
	fio_evt_t *r = (fio_evt_t *)NULL;
	if (type > 0 && f != (caf_io_file_t *)NULL) {
		r = (fio_evt_t *)xmalloc (IO_EVT_SZ);
		if (r != (fio_evt_t *)NULL) {
			if (local_caf_inotify_fd == -1) {
				local_caf_inotify_fd = inotify_init();
				if (local_caf_inotify_fd > -1) {
					r->ev_mf = f;
					r->ev_type = type;
					r->ev_sz = IO_EVENT_DATA_INOTIFY_SZ;
					r->ev_use = IO_EVENTS_INOTIFY;
					r->ev_info = f;
					r->ev_store = (io_evt_inotify_t *)xmalloc (r->ev_sz);
					r->ev_timeout = to;
					if (r->ev_info == (io_evt_inotify_t *)NULL ||
						(caf_fio_evt_init (r)) != CAF_OK) {
						xfree (r);
						r = (fio_evt_t *)NULL;
					}
				}
			}
		}
	}
	return r;
}


int
caf_fio_evt_delete (fio_evt_t *e) {
	if (e != (fio_evt_t *)NULL) {
		if ((caf_fio_evt_destroy (e)) == CAF_OK) {
			if (e->ev_info != (void *)NULL) {
				xfree (e->ev_info);
			}
			xfree (e);
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


int
caf_fio_evt_init (fio_evt_t *e) {
	io_evt_inotify_t *s;
	if (e != (fio_evt_t *)NULL) {
		s = (io_evt_inotify_t *)e->ev_info;
		if (s != (io_evt_inotify_t *)NULL) {
			e->ev_src = inotify_add_watch (local_caf_inotify_fd,
			                               e->ev_mf->path, fio_evt_events (e));
			if (e->ev_src > -1) {
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


int
caf_fio_evt_reinit (fio_evt_t *e) {
	io_evt_inotify_t *s;
	if (e != (fio_evt_t *)NULL) {
		s = (io_evt_inotify_t *)e->ev_info;
		if (s != (io_evt_inotify_t *)NULL) {
			if ((inotify_rm_watch (local_caf_inotify_fd, e->ev_src)) == 0) {
				e->ev_src = inotify_add_watch (local_caf_inotify_fd,
				                               e->ev_mf->path,
				                               fio_evt_events (e));
				if (e->ev_src > -1) {
					return CAF_OK;
				}
			}
		}
	}
	return CAF_ERROR;
}


#ifndef LINUX_SYSTEM
int
caf_fio_evt_add (fio_evt_t *e, int ev, int flg) {
	/* invalid call on inotify use */
	return CAF_ERROR;
}
#endif /* !LINUX_SYSTEM */


int
caf_fio_evt_destroy (fio_evt_t *e) {
	io_evt_inotify_t *s;
	if (e != (fio_evt_t *)NULL) {
		s = (io_evt_inotify_t *)e->ev_info;
		if (s != (io_evt_inotify_t *)NULL) {
			close (e->ev_src);
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


int
caf_fio_evt_handle (fio_evt_t *e) {
	if (e != (fio_evt_t *)NULL) {
		if (e->ev_src > -1) {
			memset (e->ev_store, 0, e->ev_sz);
			if ((read (e->ev_src, e->ev_store, e->ev_sz)) > 0) {
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


int
caf_fio_evt_isread (fio_evt_t *e) {
	int r = CAF_ERROR;
	io_evt_inotify_t *s;
	if (e != (fio_evt_t *)NULL) {
		s = (io_evt_inotify_t *)e->ev_store;
		if (s != (io_evt_inotify_t *)NULL) {
			r = s->mask & fio_evt_events_use(e, EVT_IO_READ);
		}
	}
	return r;
}


int
caf_fio_evt_iswrite (fio_evt_t *e) {
	int r = CAF_ERROR;
	io_evt_inotify_t *s;
	if (e != (fio_evt_t *)NULL) {
		s = (io_evt_inotify_t *)e->ev_store;
		if (s != (io_evt_inotify_t *)NULL) {
			r = s->mask & fio_evt_events_use(e, EVT_IO_WRITE);
		}
	}
	return r;
}

int
caf_fio_evt_isvnode (fio_evt_t *e) {
	int r = CAF_ERROR;
	io_evt_inotify_t *s;
	if (e != (fio_evt_t *)NULL) {
		s = (io_evt_inotify_t *)e->ev_store;
		if (s != (io_evt_inotify_t *)NULL) {
			r = s->mask & fio_evt_events_use(e, EVT_IO_VNODE);
		}
	}
	return r;
}

/* caf_evt_fio_inotify.c ends here */
