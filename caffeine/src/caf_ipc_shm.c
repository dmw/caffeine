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

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_ipc_shm.h"

#include <caf/caf_data_lstdl.h>


static int caf_shm_pool_delete_callback (void *data);


caf_shm_pool_t *
caf_shm_pool_new ()
{
    caf_shm_pool_t *r = (caf_shm_pool_t *)xmalloc (CAF_SHM_POOL_SZ);
    if (r != (caf_shm_pool_t *)NULL) {
        r->count = 0;
        r->sz = 0;
        r->pool = lstdl_create ();
        if (r->pool == (lstdl_t *)NULL) {
            xfree (r);
            r = (caf_shm_pool_t *)NULL;
        }
    }
    return r;
}


int
caf_shm_pool_delete (caf_shm_pool_t *s)
{
    if (s != (caf_shm_pool_t *)NULL) {
        if ((lstdl_delete (s->pool, caf_shm_pool_delete_callback)) ==
            s->count) {
            xfree (s);
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


int
caf_shm_pool_add (caf_shm_pool_t *s, caf_shm_alloc_t *a)
{
    if (s != (caf_shm_pool_t *)NULL && a != (caf_shm_alloc_t *)NULL) {
        if ((lstdl_push (s->pool, (void *)a)) != (lstdl_t *)NULL) {
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


caf_shm_alloc_t *
caf_shm_seg_new (key_t k, size_t sz, int flg)
{
    int id;
    caf_shm_alloc_t *r = (caf_shm_alloc_t *)NULL;
    if (r != (caf_shm_alloc_t *)NULL) {
        id = shmget (k, sz, flg);
        if (id > 0) {
            r = (caf_shm_alloc_t *)xmalloc (CAF_SHM_ALLOC_SZ);
            if (r != (caf_shm_alloc_t *)NULL) {
                r->id = id;
                r->key = k;
                r->sz = sz;
                r->flags = flg;
                r->ptr = CAF_SHM_BAD_ALLOC;
                return r;
            }
        }
    }
    return r;
}


int
caf_shm_seg_delete (caf_shm_alloc_t *s)
{
    if (s != (caf_shm_alloc_t *)NULL) {
        shmdt (s->ptr);
        shmctl (s->id, IPC_RMID, (struct shmid_ds *)NULL);
        xfree (s);
        return CAF_OK;
    }
    return CAF_ERROR;
}


void *
caf_shm_seg_attach (caf_shm_alloc_t *s)
{
    void *ptr = (void *)-1;
    if (s != (caf_shm_alloc_t *)NULL) {
        ptr = shmat (s->id, (void *)NULL, s->flags);
        if (ptr != CAF_SHM_BAD_ALLOC) {
            s->ptr = s;
        }
    }
    return ptr;
}


int
caf_shm_seg_detach (caf_shm_alloc_t *s)
{
    if (s != (caf_shm_alloc_t *)NULL) {
        return shmdt(s->ptr);
    }
    return CAF_ERROR;
}


int
caf_shm_seg_ctrl (caf_shm_alloc_t *s, int cmd, struct shmid_ds *b)
{
    if (s != (caf_shm_alloc_t *)NULL && cmd > 0) {
        return shmctl (s->id, cmd, b);
    }
    return CAF_ERROR;
}


static int
caf_shm_pool_delete_callback (void *data)
{
    if (data != (void *)NULL) {
        caf_shm_seg_detach ((caf_shm_alloc_t *)data);
        return caf_shm_seg_delete ((caf_shm_alloc_t *)data);
    }
    return CAF_ERROR;
}


/* caf_ipc_shm.c ends here */
