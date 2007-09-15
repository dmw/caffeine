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

#include <semaphore.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_sem_posix.h"


caf_sem_posix_t *
caf_sem_posix_new (const sem_t key, const int flag, const int value) {
	caf_sem_posix_t *r = (caf_sem_posix_t *)NULL;
	r = (caf_sem_posix_t *)xmalloc (CAF_SEM_POSIX_SZ);
	if (r != (caf_sem_posix_t *)NULL) {
		r->key = key;
		if ((sem_init (&(r->key), flag, value)) == 0) {
			r->flag = flag;
			r->value = value;
			return r;
		} else {
			xfree (r);
			r = (caf_sem_posix_t *)NULL;
		}
	}
	return r;
}


int
caf_sem_posix_delete (caf_sem_posix_t *r) {
	if (r != (caf_sem_posix_t *)NULL) {
		if ((sem_destroy (&(r->key))) == 0) {
			xfree (r);
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


caf_sem_posix_t *
caf_sem_posix_get (const sem_t k, const int f, const int v) {
	caf_sem_posix_t *r = caf_sem_posix_new (k, f, v);
	return r;
}


int
caf_sem_posix_exists (caf_sem_posix_t *r) {
	int v;
	if (r != (caf_sem_posix_t *)NULL) {
		if ((sem_getvalue (&(r->key), &v)) == 0) {
			return CAF_OK;
		} else {
			return CAF_ERROR;
		}
	}
	return CAF_ERROR;
}


int
caf_sem_posix_post (caf_sem_posix_t *r) {
	if (r != (caf_sem_posix_t *)NULL) {
		return sem_post (&(r->key));
	}
	return CAF_ERROR;
}


int
caf_sem_posix_getvalue (caf_sem_posix_t *r) {
	if (r != (caf_sem_posix_t *)NULL) {
		return sem_getvalue (&(r->key), &(r->value));
	}
	return CAF_ERROR;
}


int
caf_sem_posix_wait (caf_sem_posix_t *r) {
	if (r != (caf_sem_posix_t *)NULL) {
		return sem_wait (&(r->key));
	}
	return CAF_ERROR;
}


int
caf_sem_posix_trywait (caf_sem_posix_t *r) {
	if (r != (caf_sem_posix_t *)NULL) {
		return sem_trywait (&(r->key));
	}
	return CAF_ERROR;
}


int
caf_sem_posix_lock (caf_sem_posix_t *r) {
	return caf_sem_posix_wait (r);
}


int
caf_sem_posix_trylock (caf_sem_posix_t *r) {
	return caf_sem_posix_trywait (r);
}

int
caf_sem_posix_unlock (caf_sem_posix_t *r) {
	return caf_sem_posix_post (r);
}

/* caf_sem_posix.c ends here */
