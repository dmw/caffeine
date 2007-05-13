/* -*- mode: c; c-default-style: "k&r"; indent-tabs-mode: nil; -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 expandtab: */
/* kate: space-indent on; indent-width 4; mixedindent off; indent-mode none; */
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
#include "caf/caf_evt_fio.h"


static caf_evt_fio_event_mapping_t fio_evt_mapping[] = {
    /* read events */
    {
        EVT_IO_READ_IDX,
#ifdef BSD_SYSTEM
        EVFILT_READ,
#else
        0,
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
        IN_ACCESS
#else
        0
#endif /* !LINUX_SYSTEM */
    },
    /* write events */
    {
        EVT_IO_WRITE_IDX,
#ifdef BSD_SYSTEM
        EVFILT_WRITE,
#else
        0,
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
        IN_CREATE | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE
#else
        0
#endif /* !ILINUX_SYSTEM */
    },
    /* vnode events */
    {
        EVT_IO_VNODE_IDX,
#ifdef BSD_SYSTEM
        EVFILT_VNODE,
#else
        0,
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
        IN_ATTRIB | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF | IN_MOVED_FROM
        | IN_MOVED_TO
#else
        0
#endif /* !ILINUX_SYSTEM */
    }
};


int
fio_evt_events (fio_evt_t *e)
{
    int evt = 0;
    if (e != (fio_evt_t *)NULL) {
        switch (e->ev_use) {
#ifdef BSD_SYSTEM
        case IO_EVENTS_KEVENT:
            if (e->ev_type & EVT_IO_READ) {
                evt |= fio_evt_mapping[EVT_IO_READ_IDX].evt_kevent;
            }
            if (e->ev_type & EVT_IO_WRITE) {
                evt |= fio_evt_mapping[EVT_IO_WRITE_IDX].evt_kevent;
            }
            if (e->ev_type & EVT_IO_VNODE) {
                evt |= fio_evt_mapping[EVT_IO_VNODE_IDX].evt_kevent;
            }
            return evt;
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
        case IO_EVENTS_INOTIFY:
            if (e->ev_type & EVT_IO_READ) {
                evt |= fio_evt_mapping[EVT_IO_READ_IDX].evt_inotify;
            }
            if (e->ev_type & EVT_IO_WRITE) {
                evt |= fio_evt_mapping[EVT_IO_WRITE_IDX].evt_inotify;
            }
            if (e->ev_type & EVT_IO_VNODE) {
                evt |= fio_evt_mapping[EVT_IO_VNODE_IDX].evt_inotify;
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
fio_evt_events_use (fio_evt_t *e, fio_evt_use_t use)
{
    int evt = 0;
    if (e != (fio_evt_t *)NULL) {
        switch (e->ev_use) {
#ifdef BSD_SYSTEM
        case IO_EVENTS_KEVENT:
            if (use & EVT_IO_READ) {
                evt |= fio_evt_mapping[EVT_IO_READ_IDX].evt_kevent;
            }
            if (use & EVT_IO_WRITE) {
                evt |= fio_evt_mapping[EVT_IO_WRITE_IDX].evt_kevent;
            }
            if (use & EVT_IO_VNODE) {
                evt |= fio_evt_mapping[EVT_IO_VNODE_IDX].evt_kevent;
            }
            return evt;
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
        case IO_EVENTS_INOTIFY:
            if (use & EVT_IO_READ) {
                evt |= fio_evt_mapping[EVT_IO_READ_IDX].evt_inotify;
            }
            if (use & EVT_IO_WRITE) {
                evt |= fio_evt_mapping[EVT_IO_WRITE_IDX].evt_inotify;
            }
            if (use & EVT_IO_VNODE) {
                evt |= fio_evt_mapping[EVT_IO_VNODE_IDX].evt_inotify;
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
