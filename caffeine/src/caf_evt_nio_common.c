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
#include <errno.h>

#ifdef LINUX
#include <sys/poll.h>s
#ifdef IO_EVENT_USE_EPOLL
#include <sys/epoll.h>
#endif /* !IO_EVENT_USE_EPOLL */
#else
#include <poll.h>
#endif /* !LINUX */

#include <sys/select.h>

#ifdef BSD_SYSTEM
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#endif /* !BSD */

#ifdef LINUX_SYSTEM
#include <sys/inotify.h>
#endif /* !LINUX */

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_evt_nio.h"


static caf_evt_io_event_mapping_t io_evt_mapping[] = {
            /* read events */
            {
                EVT_IO_READ_IDX,
                POLLIN | POLLPRI,
                EVT_IO_READ,
#ifdef BSD_SYSTEM
                EVFILT_READ,
#else
                0,
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
                EPOLLIN | EPOLLPRI
#else
                0
#endif /* !LINUX_SYSTEM */
            },
            /* write events */
            {
                EVT_IO_WRITE_IDX,
                POLLOUT,
                EVT_IO_WRITE,
#ifdef BSD_SYSTEM
                EVFILT_WRITE,
#else
                0,
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
                EPOLLOUT
#else
                0
#endif /* !ILINUX_SYSTEM */
            }
        };


int
io_evt_events (io_evt_t *e) {
	int evt = 0;
	if (e != (io_evt_t *)NULL) {
		switch (e->ev_use) {
		case IO_EVENTS_SELECT:
			if (e->ev_type & EVT_IO_READ) {
				evt |= io_evt_mapping[EVT_IO_READ_IDX].evt_select;
			}
			if (e->ev_type & EVT_IO_WRITE) {
				evt |= io_evt_mapping[EVT_IO_WRITE_IDX].evt_select;
			}
			return evt;
		case IO_EVENTS_POLL:
			if (e->ev_type & EVT_IO_READ) {
				evt |= io_evt_mapping[EVT_IO_READ_IDX].evt_poll;
			}
			if (e->ev_type & EVT_IO_WRITE) {
				evt |= io_evt_mapping[EVT_IO_WRITE_IDX].evt_poll;
			}
			return evt;
#ifdef BSD_SYSTEM
		case IO_EVENTS_KEVENT:
			if (e->ev_type & EVT_IO_READ) {
				evt |= io_evt_mapping[EVT_IO_READ_IDX].evt_kevent;
			}
			if (e->ev_type & EVT_IO_WRITE) {
				evt |= io_evt_mapping[EVT_IO_WRITE_IDX].evt_kevent;
			}
			return evt;
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
		case IO_EVENTS_EPOLL:
			if (e->ev_type & EVT_IO_READ) {
				evt |= io_evt_mapping[EVT_IO_READ_IDX].evt_epoll;
			}
			if (e->ev_type & EVT_IO_WRITE) {
				evt |= io_evt_mapping[EVT_IO_WRITE_IDX].evt_epoll;
			}
			return evt;
#endif /* !LINUX_SYSTEM */
		default:
			return evt;
		}
	}
	return evt;
}


int
io_evt_events_use (io_evt_t *e, io_evt_use_t use) {
	int evt = 0;
	if (e != (io_evt_t *)NULL) {
		switch (e->ev_use) {
		case IO_EVENTS_SELECT:
			if (use & EVT_IO_READ) {
				evt |= io_evt_mapping[EVT_IO_READ_IDX].evt_select;
			}
			if (use & EVT_IO_WRITE) {
				evt |= io_evt_mapping[EVT_IO_WRITE_IDX].evt_select;
			}
			return evt;
		case IO_EVENTS_POLL:
			if (use & EVT_IO_READ) {
				evt |= io_evt_mapping[EVT_IO_READ_IDX].evt_poll;
			}
			if (use & EVT_IO_WRITE) {
				evt |= io_evt_mapping[EVT_IO_WRITE_IDX].evt_poll;
			}
			return evt;
#ifdef BSD_SYSTEM
		case IO_EVENTS_KEVENT:
			if (use & EVT_IO_READ) {
				evt |= io_evt_mapping[EVT_IO_READ_IDX].evt_kevent;
			}
			if (use & EVT_IO_WRITE) {
				evt |= io_evt_mapping[EVT_IO_WRITE_IDX].evt_kevent;
			}
			return evt;
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
		case IO_EVENTS_EPOLL:
			if (use & EVT_IO_READ) {
				evt |= io_evt_mapping[EVT_IO_READ_IDX].evt_epoll;
			}
			if (use & EVT_IO_WRITE) {
				evt |= io_evt_mapping[EVT_IO_WRITE_IDX].evt_epoll;
			}
			return evt;
#endif /* !LINUX_SYSTEM */
		default:
			return evt;
		}
	}
	return evt;
}


/* caf_evt_io_common.c ends here */
