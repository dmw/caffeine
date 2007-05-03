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

#define IO_EVENT_USE_INOTIFY
#include "caf/caf_evt_fio.h"

static int caf_inotify_init = 0;


fio_evt_t *
caf_fio_evt_new  (int fd, int type, int to)
{
    fio_evt_t *r = (fio_evt_t *)NULL;
    return r;
}


int
caf_fio_evt_delete (fio_evt_t *e)
{
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
caf_fio_evt_init (fio_evt_t *e)
{
    return CAF_ERROR;
}


int
caf_fio_evt_reinit (fio_evt_t *e)
{
    return CAF_ERROR;
}


int
caf_fio_evt_add (fio_evt_t *e, int ev)
{
    return CAF_ERROR;
}


int
caf_fio_evt_destroy (fio_evt_t *e)
{
    return CAF_ERROR;
}


int
caf_fio_evt_handle (fio_evt_t *e)
{
    return r;
}


int
caf_fio_evt_isread (fio_evt_t *e)
{
    return r;
}


int
caf_fio_evt_iswrite (fio_evt_t *e)
{
    return r;
}


int
caf_fio_evt_isvnode (fio_evt_t *e)
{
    return r;
}

/* caf_evt_fio_inotify.c ends here */
