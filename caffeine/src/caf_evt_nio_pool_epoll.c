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
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"

#define IO_EVENT_USE_EPOLL
#include "caf/caf_evt_nio.h"
#include "caf/caf_evt_nio_pool.h"


io_evt_pool_epoll_t *
io_evt_pool_epoll_new (int cnt, int tos, int ton)
{
    io_evt_pool_epoll_t *r = (io_evt_pool_epoll_t *)NULL;
    if (cnt > 0) {
        r = (io_evt_pool_epoll_t *)xmalloc (IO_EVENT_DATA_POOL_POLL_SZ);
        if (r != (io_evt_pool_epoll_t *)NULL) {
            memset ((void *)r, 0, IO_EVENT_DATA_POOL_POLL_SZ);
            r->epoll_count = cnt;
            r->epoll_sz = (size_t)cnt * IO_EVENT_DATA_EPOLLS_SZ;
            r->epoll = (struct epoll_event *)xmalloc (r->epoll_sz);
            r->repoll = (struct epoll_event *)xmalloc (r->epoll_sz);
            if (r->epoll != (struct epoll_event *)NULL &&
                r->repoll != (struct epoll_event *)NULL) {
                memset ((void *)r->epoll, 0, r->epoll_sz);
                memset ((void *)r->repoll, 0, r->epoll_sz);
                r->efd = epoll_create (r->epoll_count);
                if (r->efd >= 0) {
                    caf_io_evt_pool_reset (r);
                    r->timeout.tv_sec = tos;
                    r->timeout.tv_nsec = ton;
                } else {
                    xfree (r);
                    r = (io_evt_pool_epoll_t *)NULL;
                }
            } else {
                if (r->epoll != (struct epoll_event *)NULL) {
                    xfree (r->epoll);
                }
                if (r->repoll != (struct epoll_event *)NULL) {
                    xfree (r->repoll);
                }
                xfree (r);
                r = (io_evt_pool_epoll_t *)NULL;
            }
        }
    }
    return r;
}


int
io_evt_pool_epoll_delete (io_evt_pool_epoll_t *r)
{
    if (r != (io_evt_pool_epoll_t *)NULL) {
        if (r->efd >= 0) {
            close (r->efd);
        }
        if (r->epoll != (struct epoll_event *)NULL) {
            xfree (r->epoll);
        }
        if (r->repoll != (struct epoll_event *)NULL) {
            xfree (r->repoll);
        }
        xfree (r);
        return CAF_OK;
    }
    return CAF_ERROR;
}


int
io_evt_pool_epoll_reset (io_evt_pool_epoll_t *e)
{
    int i;
    if (e != (io_evt_pool_epoll_t *)NULL) {
        if (e->epoll != (struct epoll_event *)NULL) {
            for (i = 0; i < e->epoll_count; i++) {
                e->epoll[i].events = 0;
                e->epoll[i].data.fd = -1;
            }
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


int
io_evt_pool_epoll_add (int fd, io_evt_pool_epoll_t *e, int ef)
{
    int i, r = -1;
    if (e != (io_evt_pool_epoll_t *)NULL) {
        if (e->epoll != (struct epoll_event *)NULL) {
            for (i = 0; i < e->epoll_count; i++) {
                if (e->epoll[i].data.fd == -1) {
                    r = i;
                    i = e->epoll_count;
                }
            }
            if (r >= 0) {
                e->epoll[r].data.fd = fd;
                e->epoll[r].events = ef;
                if ((epoll_ctl (e->efd, EPOLL_CTL_ADD, fd, &(e->epoll[r]))) <
                    0) {
                    e->epoll[r].data.fd = -1;
                    e->epoll[r].events = 0;
                    return CAF_ERROR;
                } else {
                    return CAF_OK;
                }
            }
        }
    }
    return CAF_ERROR;
}


int
io_evt_pool_epoll_hasevent (int fd, io_evt_pool_epoll_t *e, int ef)
{
    int i;
    if (e != (io_evt_pool_epoll_t *)NULL) {
        if (e->epoll != (struct epoll_event *)NULL) {
            for (i = 0; i < e->epoll_count; i++) {
                if (fd == e->repoll[i].data.fd) {
                    return (e->repoll[i].events & ef) ? CAF_OK : CAF_ERROR;
                }
            }
        }
    }
    return CAF_ERROR;
}


int
io_evt_pool_epoll_getevent (int fd, io_evt_pool_epoll_t *e)
{
    int i;
    if (e != (io_evt_pool_epoll_t *)NULL) {
        if (e->epoll != (struct epoll_event *)NULL) {
            for (i = 0; i < e->epoll_count; i++) {
                if (fd == e->repoll[i].data.fd) {
                    return e->repoll[i].events;
                }
            }
        }
    }
    return CAF_ERROR;
}


int
io_evt_pool_epoll_etype (int fd, io_evt_pool_epoll_t *e)
{
    int i, r = 0;
    int wre = POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI;
    int wwe = POLLOUT | POLLWRNORM | POLLWRBAND;
    if (e != (io_evt_pool_epoll_t *)NULL) {
        if (e->epoll != (struct epoll_event *)NULL) {
            for (i = 0; i < e->epoll_count; i++) {
                if (fd == e->repoll[i].data.fd) {
                    r |= (e->repoll[i].events & wre) ? EVT_IO_READ : 0;
                    r |= (e->repoll[i].events & wwe) ? EVT_IO_WRITE : 0;
                }
            }
        }
    }
    return r;
}


int
io_evt_pool_epoll_handle (io_evt_pool_epoll_t *e)
{
    int n = 0;
    if (e != (io_evt_pool_epoll_t *)NULL) {
        if (e->epoll != (struct epoll_event *)NULL) {
            n = epoll_wait (e->efd, e->epoll, (nfds_t)e->epoll_count,
                            e->timeout.tv_sec);
            return (n > 0) ? CAF_OK : CAF_ERROR;
        }
    }
    return CAF_ERROR;
}

/* caf_evt_io_pool_poll.c ends here */
