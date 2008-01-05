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

#ifdef HAVE_CONFIG_H
#include "caf/config.h"
#endif /* !HAVE_CONFIG_H */

#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_thread_mutex.h"


pth_mutex_t *
pth_mtx_new (void) {
	pth_mutex_t *mtx = (pth_mutex_t *)NULL;
	mtx = (pth_mutex_t *)xmalloc (CAF_PTH_MTX_SZ);
	if (mtx != (pth_mutex_t *)NULL) {
		mtx->at = 0;
	}
	return mtx;
}


void
pth_mtx_delete (pth_mutex_t *mtx) {
	if (mtx != (pth_mutex_t *)NULL) {
		xfree (mtx);
	}
}


int
pth_mtxattr_init (pth_mutex_t *mtx) {
	if (mtx != (pth_mutex_t *)NULL) {
		return pthread_mutexattr_init(&(mtx->attr));
	}
	return CAF_ERROR_SUB;
}


int
pth_mtxattr_destroy (pth_mutex_t *mtx) {
	if (mtx != (pth_mutex_t *)NULL) {
		return pthread_mutexattr_destroy(&(mtx->attr));
	}
	return CAF_ERROR_SUB;
}


int
pth_mtx_init (pth_mutex_t *mtx) {
	if (mtx != (pth_mutex_t *)NULL) {
		return pthread_mutex_init(&(mtx->mutex), &(mtx->attr));
	}
	return CAF_ERROR_SUB;
}


int
pth_mtx_destroy (pth_mutex_t *mtx) {
	if (mtx != (pth_mutex_t *)NULL) {
		return pthread_mutex_destroy(&(mtx->mutex));
	}
	return CAF_ERROR_SUB;
}


int
pth_mtxattr_set (pth_mutex_t *mtx, pth_mutexattr_types_t t, int data) {
	if (mtx != (pth_mutex_t *)NULL) {
		switch (t) {
			/* pthread_mutexattr_setprioceiling */
		case PTHDR_MUTEX_PRIOCEIL:
			mtx->at |= t;
			return pthread_mutexattr_setprioceiling (&(mtx->attr), data);
			/* pthread_mutexattr_setprotocol */
		case PTHDR_MUTEX_PROTO:
			mtx->at |= t;
			return pthread_mutexattr_setprotocol (&(mtx->attr), data);
			/* pthread_mutexattr_settype */
		case PTHDR_MUTEX_TYPE:
			mtx->at |= t;
			return pthread_mutexattr_settype (&(mtx->attr), data);
			/* lock */
		case PTHDR_MUTEX_LOCK:
			mtx->at |= PTHDR_MUTEX_LOCK;
			return CAF_OK;
		default:
			return CAF_ERROR_SUB;
		}
	}
	return CAF_ERROR_SUB;
}


int
pth_mtxattr_get (pth_mutex_t *mtx, pth_mutexattr_types_t t, int *data) {
	if (mtx != (pth_mutex_t *)NULL) {
		switch (t) {
			/* pthread_mutexattr_getprioceiling */
		case PTHDR_MUTEX_PRIOCEIL:
			return pthread_mutexattr_getprioceiling (&(mtx->attr), data);
			/* pthread_mutexattr_getprotocol */
		case PTHDR_MUTEX_PROTO:
			return pthread_mutexattr_getprotocol (&(mtx->attr), data);
			/* pthread_mutexattr_gettype */
		case PTHDR_MUTEX_TYPE:
			return pthread_mutexattr_gettype (&(mtx->attr), data);
			/* lock */
		case PTHDR_MUTEX_LOCK:
			return (mtx->at & t);
		default:
			return CAF_ERROR_SUB;
		}
	}
	return CAF_ERROR_SUB;
}


int
pth_mtx_trylock (pth_mutex_t *mtx) {
	if (mtx != (pth_mutex_t *)NULL) {
		if ((pthread_mutex_trylock (&(mtx->mutex))) == 0) {
			return pth_mtxattr_set (mtx, PTHDR_MUTEX_LOCK, 0);
		}
	}
	return CAF_ERROR;
}


int
pth_mtx_lock (pth_mutex_t *mtx) {
	if (mtx != (pth_mutex_t *)NULL) {
		if ((pthread_mutex_lock (&(mtx->mutex))) == 0) {
			return pth_mtxattr_set (mtx, PTHDR_MUTEX_LOCK, 0);
		}
	}
	return CAF_ERROR;
}


int
pth_mtx_unlock (pth_mutex_t *mtx) {
	if (mtx != (pth_mutex_t *)NULL) {
		if (pthread_mutex_unlock (&(mtx->mutex)) == 0) {
			mtx->at &= ~PTHDR_MUTEX_LOCK;
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}

/* caf_thread_mutex.c ends here */

