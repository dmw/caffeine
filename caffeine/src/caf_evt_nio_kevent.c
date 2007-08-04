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

#include <sys/types.h>
#include <unistd.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"

#define IO_EVENT_USE_KEVENT
#include "caf/caf_evt_nio.h"

#define KEVENT_FILTER               (EV_ADD|EV_ENABLE)
#define KEVENT_FILTER_COUNT         1

io_evt_t *
caf_io_evt_new  (int fd, int type, int to) {
	io_evt_t *r = (io_evt_t *)NULL;
	if (fd > -1 && type > 0) {
		r = (io_evt_t *)xmalloc (IO_EVT_SZ);
		if (r != (io_evt_t *)NULL) {
			r->ev_mfd = fd;
			r->ev_use = IO_EVENTS_KEVENT;
			r->ev_type = type;
			r->ev_sz = IO_EVENT_DATA_KEVENT_SZ;
			r->ev_info = (io_evt_kevent_t *)xmalloc (r->ev_sz);
			r->ev_store = (io_evt_kevent_t *)xmalloc (r->ev_sz);
			r->ev_timeout = to;
			if (r->ev_info == (io_evt_kevent_t *)NULL ||
			        (caf_io_evt_init (r)) != CAF_OK) {
				xfree (r);
				r = (io_evt_t *)NULL;
			}
		}
	}
	return r;
}


int
caf_io_evt_delete (io_evt_t *e) {
	if (e != (io_evt_t *)NULL) {
		if ((caf_io_evt_destroy (e)) == CAF_OK) {
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
caf_io_evt_init (io_evt_t *e) {
	io_evt_kevent_t *s;
	if (e != (io_evt_t *)NULL) {
		s = (io_evt_kevent_t *)e->ev_info;
		if (s != (io_evt_kevent_t *)NULL) {
			e->ev_src = kqueue ();
			if (e->ev_src > -1) {
				if (e->ev_type & EVT_IO_READ) {
					EV_SET(s, e->ev_mfd, EVFILT_READ, KEVENT_FILTER, 0, 0, 0);
				}
				if (e->ev_type & EVT_IO_WRITE) {
					EV_SET(s, e->ev_mfd, EVFILT_WRITE, KEVENT_FILTER, 0, 0, 0);
				}
				if ((kevent (e->ev_src, s, KEVENT_FILTER_COUNT, NULL, 0, NULL))
				        >= 0) {
					return CAF_OK;
				}
			}
		}
	}
	return CAF_ERROR;
}


int
caf_io_evt_reinit (io_evt_t *e) {
	io_evt_kevent_t *s;
	if (e != (io_evt_t *)NULL) {
		s = (io_evt_kevent_t *)e->ev_info;
		if (s != (io_evt_kevent_t *)NULL) {
			if (e->ev_src > -1) {
				if (e->ev_type & EVT_IO_READ) {
					EV_SET(s, e->ev_mfd, EVFILT_READ, KEVENT_FILTER, 0, 0, 0);
				}
				if (e->ev_type & EVT_IO_WRITE) {
					EV_SET(s, e->ev_mfd, EVFILT_WRITE, KEVENT_FILTER, 0, 0, 0);
				}
				if ((kevent (e->ev_src, s, KEVENT_FILTER_COUNT, NULL, 0, NULL))
				        >= 0) {
					return CAF_OK;
				}
			}
		}
	}
	return CAF_ERROR;
}


int
caf_io_evt_add (io_evt_t *e, int ev, int flg) {
	io_evt_kevent_t *s;
	if (e != (io_evt_t *)NULL) {
		s = (io_evt_kevent_t *)e->ev_info;
		if (s != (io_evt_kevent_t *)NULL) {
			if (e->ev_src > -1) {
				EV_SET(s, e->ev_mfd, ev, KEVENT_FILTER, flg, 0, 0);
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


int
caf_io_evt_destroy (io_evt_t *e) {
	io_evt_kevent_t *s;
	if (e != (io_evt_t *)NULL) {
		s = (io_evt_kevent_t *)e->ev_info;
		if (s != (io_evt_kevent_t *)NULL) {
			close (e->ev_src);
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


int
caf_io_evt_handle (io_evt_t *e) {
	int r = CAF_ERROR;
	io_evt_kevent_t *s;
	struct timespec ts;
	if (e != (io_evt_t *)NULL) {
		s = (io_evt_kevent_t *)e->ev_info;
		if (s != (io_evt_kevent_t *)NULL) {
			ts.tv_sec = e->ev_timeout;
			ts.tv_nsec = 0;
			if ((kevent (e->ev_src, NULL, 0, s, KEVENT_FILTER_COUNT, &ts))
			        == 1) {
				r = CAF_OK;
			}
		}
	}
	return r;
}


int
caf_io_evt_isread (io_evt_t *e) {
	int r = CAF_ERROR;
	io_evt_kevent_t *s;
	if (e != (io_evt_t *)NULL) {
		s = (io_evt_kevent_t *)e->ev_store;
		if (s != (io_evt_kevent_t *)NULL) {
			r = s->filter & EVFILT_READ ? CAF_OK : CAF_ERROR;
		}
	}
	return r;
}


int
caf_io_evt_iswrite (io_evt_t *e) {
	int r = CAF_ERROR;
	io_evt_kevent_t *s;
	if (e != (io_evt_t *)NULL) {
		s = (io_evt_kevent_t *)e->ev_store;
		if (s != (io_evt_kevent_t *)NULL) {
			r = s->filter & EVFILT_WRITE ? CAF_OK : CAF_ERROR;
		}
	}
	return r;
}


/* caf_io_evt_kevent.c ends here */
