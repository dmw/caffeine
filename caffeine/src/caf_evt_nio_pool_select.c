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

#define IO_EVENT_USE_SELECT
#include "caf/caf_evt_nio.h"
#include "caf/caf_evt_nio_pool.h"


io_evt_pool_select_t *
io_evt_pool_select_new (int cnt, int tos, int ton)
{
    io_evt_pool_select_t *r = (io_evt_pool_select_t *)NULL;
    if (cnt > 0) {
        /* cnt is ignored */
        r = (io_evt_pool_select_t *)xmalloc (IO_EVENT_DATA_POOL_SELECT_SZ);
        if (r != (io_evt_pool_select_t *)NULL) {
            memset ((void *)r, 0, IO_EVENT_DATA_POOL_SELECT_SZ);
            r->timeout.tv_sec = tos;
            r->timeout.tv_usec = ton;
        }
    }
    return r;
}


int
io_evt_pool_select_delete (io_evt_pool_select_t *r)
{
    if (r != (io_evt_pool_select_t *)NULL) {
        xfree (r);
        return CAF_OK;
    }
    return CAF_ERROR;
}


int
io_evt_pool_select_reset (io_evt_pool_select_t *e)
{
    if (e != (io_evt_pool_select_t *)NULL) {
        FD_ZERO(&(e->rd));
        FD_ZERO(&(e->wr));
        return CAF_OK;
    }
    return CAF_ERROR;
}


int
io_evt_pool_select_add (int fd, io_evt_pool_select_t *e, int ef)
{
    if (e != (io_evt_pool_select_t *)NULL) {
        if (e->fd < fd) {
            e->fd = fd;
        }
        if (ef & EVT_IO_READ) {
            FD_CLR(fd, &(e->rd));
            FD_SET(fd, &(e->rd));
        }
        if (ef & EVT_IO_WRITE) {
            FD_CLR(fd, &(e->wr));
            FD_SET(fd, &(e->wr));
        }
        return CAF_OK;
    }
    return CAF_ERROR;
}


int
io_evt_pool_select_hasevent (int fd, io_evt_pool_select_t *e, int ef)
{
    int r = 0;
    if (e != (io_evt_pool_select_t *)NULL) {
        if (ef == EVT_IO_READ) {
            r = FD_ISSET(fd, &(e->rd)) ? CAF_OK : CAF_ERROR;
        }
        if (ef == EVT_IO_WRITE) {
            r = FD_ISSET(fd, &(e->wr)) ? CAF_OK : CAF_ERROR;
        }
        return r;
    }
    return CAF_ERROR;
}


int
io_evt_pool_select_getevent (int fd, io_evt_pool_select_t *e)
{
    int r = 0;
    if (e != (io_evt_pool_select_t *)NULL) {
        r |= FD_ISSET(fd, &(e->rd)) ? EVT_IO_READ : 0;
        r |= FD_ISSET(fd, &(e->wr)) ? EVT_IO_WRITE : 0;
    }
    return r;
}


int
io_evt_pool_select_etype (int fd, io_evt_pool_select_t *e)
{
    int r = 0;
    if (e != (io_evt_pool_select_t *)NULL) {
        r |= FD_ISSET(fd, &(e->rd)) ? EVT_IO_READ : 0;
        r |= FD_ISSET(fd, &(e->wr)) ? EVT_IO_WRITE : 0;
    }
    return r;
}


int
io_evt_pool_select_handle (io_evt_pool_select_t *e)
{
    int n = 0;
    if (e != (io_evt_pool_select_t *)NULL) {
        n = select (e->fd, &(e->rd), &(e->wr), NULL, &(e->timeout));
        return (n > 0) ? CAF_OK : CAF_ERROR;
    }
    return CAF_ERROR;
}

/* caf_evt_io_pool_select.c ends here */
