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
static char Id[] = "$Id: caf_thread_rwlock.c 200 2007-02-17 23:15:30Z dmw $";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_thread_rwlock.h"


pth_rwlock_t *
pth_rwl_new (int id)
{
    pth_rwlock_t *rwl = (pth_rwlock_t *)NULL;
    rwl = (pth_rwlock_t *)xmalloc (CAF_PTH_RWLOCK_T_SZ);
    if (rwl != (pth_rwlock_t *)NULL) {
        rwl->id = id;
        rwl->at = 0;
    }
    return rwl;
}


void
pth_rwl_delete (pth_rwlock_t *rwl)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        xfree (rwl);
    }
}


int
pth_rwlattr_init (pth_rwlock_t *rwl)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        return pthread_rwlockattr_init(&(rwl->attr));
    }
    return CAF_ERROR_SUB;
}


int
pth_rwlattr_destroy (pth_rwlock_t *rwl)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        return pthread_rwlockattr_destroy(&(rwl->attr));
    }
    return CAF_ERROR_SUB;
}


int
pth_rwl_init (pth_rwlock_t *rwl)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        return pthread_rwlock_init(&(rwl->rwlock), &(rwl->attr));
    }
    return CAF_ERROR_SUB;
}


int
pth_rwl_destroy (pth_rwlock_t *rwl)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        return pthread_rwlock_destroy(&(rwl->rwlock));
    }
    return CAF_ERROR_SUB;
}


int
pth_rwlattr_set (pth_rwlock_t *rwl, pth_rwlock_types_t t, int data)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        switch (t) {
        /* pthread_rwlockattr_setpshared */
        case PTHDR_RWLOCK_PSHARED:
            rwl->at |= t;
            return pthread_rwlockattr_setpshared (&(rwl->attr), data);
        /* pthread_rwlockattr_setpshared */
        case PTHDR_RWLOCK_PPRIVATE:
            rwl->at |= t;
            return pthread_rwlockattr_setpshared (&(rwl->attr), data);
        /* read/write lock */
        case PTHDR_RWLOCK_RDLOCK:
        case PTHDR_RWLOCK_WRLOCK:
        case PTHDR_RWLOCK_TRDLOCK:
        case PTHDR_RWLOCK_TWRLOCK:
            rwl->at |= t;
            return CAF_OK;
        default:
            return CAF_ERROR_SUB;
        }
    }
    return CAF_ERROR_SUB;
}


int
pth_rwlattr_get (pth_rwlock_t *rwl, pth_rwlock_types_t t, int *data)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        switch (t) {
        /* pthread_rwlockattr_getpshared */
        case PTHDR_RWLOCK_PSHARED:
            return pthread_rwlockattr_getpshared (&(rwl->attr), data);
        /* pthread_rwlockattr_getpshared */
        case PTHDR_RWLOCK_PPRIVATE:
            return pthread_rwlockattr_getpshared (&(rwl->attr), data);
        /* read/write lock */
        case PTHDR_RWLOCK_RDLOCK:
        case PTHDR_RWLOCK_WRLOCK:
        case PTHDR_RWLOCK_TRDLOCK:
        case PTHDR_RWLOCK_TWRLOCK:
            return (rwl->at & t);
        default:
            return CAF_ERROR_SUB;
        }
    }
    return CAF_ERROR_SUB;
}


int
pth_rwl_wrlock (pth_rwlock_t *rwl, int tl, const struct timespec *to)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        if (to != (struct timespec *)NULL) {
            if ((pthread_rwlock_timedwrlock (&(rwl->rwlock), to)) == 0) {
                return pth_rwlattr_set (rwl, PTHDR_RWLOCK_TWRLOCK, 0);
            }
        } else if (tl != 0) {
            if ((pthread_rwlock_trywrlock (&(rwl->rwlock))) == 0) {
                return pth_rwlattr_set (rwl, PTHDR_RWLOCK_WRLOCK, 0);
            }
        } else {
            if ((pthread_rwlock_wrlock (&(rwl->rwlock))) == 0) {
                return pth_rwlattr_set (rwl, PTHDR_RWLOCK_WRLOCK, 0);
            }
        }
    }
    return CAF_ERROR_SUB;
}


int
pth_rwl_rdlock (pth_rwlock_t *rwl, int tl, const struct timespec *to)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        if (to != (struct timespec *)NULL) {
            if ((pthread_rwlock_timedrdlock (&(rwl->rwlock), to)) == 0) {
                return pth_rwlattr_set (rwl, PTHDR_RWLOCK_TRDLOCK, 0);
            }
        } else if (tl != 0) {
            if ((pthread_rwlock_tryrdlock (&(rwl->rwlock))) == 0) {
                return pth_rwlattr_set (rwl, PTHDR_RWLOCK_RDLOCK, 0);
            }
        } else {
            if ((pthread_rwlock_rdlock (&(rwl->rwlock))) == 0) {
                return pth_rwlattr_set (rwl, PTHDR_RWLOCK_RDLOCK, 0);
            }
        }
    }
    return CAF_ERROR_SUB;
}


int
pth_rwl_unlock (pth_rwlock_t *rwl)
{
    if (rwl != (pth_rwlock_t *)NULL) {
        if ((pthread_rwlock_unlock (&(rwl->rwlock))) == 0) {
            rwl->at &= ~CAF_ALL_RWLOCKS;
            return CAF_OK;
        }
    }
    return CAF_ERROR_SUB;
}

/* caf_thread_rwlock.c ends here */
