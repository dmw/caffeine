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
#include <sys/types.h>
#include <pthread.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_thread_cond.h"


pth_cond_t *
pth_cond_new (void) {
	pth_cond_t *c = (pth_cond_t *)xmalloc (PTH_COND_SZ);
	if (c != (pth_cond_t *)NULL) {
		c->at = 0;
	}
	return c;
}


int
pth_cond_delete (pth_cond_t *c) {
	if (c != (pth_cond_t *)NULL) {
		xfree (c);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
pth_cond_init (pth_cond_t *c) {
	if (c != (pth_cond_t *)NULL) {
		return pthread_cond_init (&(c->cond), &(c->attr));
	}
	return CAF_ERROR;
}


int
pth_cond_destroy (pth_cond_t *c) {
	if (c != (pth_cond_t *)NULL) {
		return pthread_cond_destroy (&(c->cond));
	}
	return CAF_ERROR;
}


int
pth_condattr_init (pth_cond_t *c) {
	if (c != (pth_cond_t *)NULL) {
		return pthread_condattr_init (&(c->attr));
	}
	return CAF_ERROR;
}


int
pth_condattr_destroy (pth_cond_t *c) {
	if (c != (pth_cond_t *)NULL) {
		return pthread_condattr_destroy (&(c->attr));
	}
	return CAF_ERROR;
}


pth_cond_t *
pth_condi_init (void) {
	pth_cond_t *c = pth_cond_new ();
	if (c != (pth_cond_t *)NULL) {
		if ((pth_condattr_init (c)) == CAF_OK) {
			if ((pth_cond_init (c)) == CAF_OK) {
				return c;
			} else {
				if ((pth_cond_destroy (c)) == CAF_OK) {
					pth_cond_delete (c);
					c = (pth_cond_t *)NULL;
				}
			}
		} else {
			pth_cond_delete (c);
			c = (pth_cond_t *)NULL;
		}
	}
	return c;
}


int
pth_condi_delete (pth_cond_t *c) {
	if (c != (pth_cond_t *)NULL) {
		if ((pth_condattr_destroy (c)) == CAF_OK) {
			if ((pth_cond_destroy (c)) == CAF_OK) {
				return pth_cond_delete (c);
			}
		}
	}
	return CAF_ERROR;
}


int
pth_cond_signal (pth_cond_t *c) {
	if (c != (pth_cond_t *)NULL) {
		return pthread_cond_signal (&(c->cond));
	}
	return CAF_ERROR;
}


int
pth_cond_broadcast (pth_cond_t *c) {
	if (c != (pth_cond_t *)NULL) {
		return pthread_cond_broadcast (&(c->cond));
	}
	return CAF_ERROR;
}


int
pth_cond_wait (pth_cond_t *c, pth_mutex_t *m) {
	if (c != (pth_cond_t *)NULL) {
		pthread_cond_wait (&(c->cond), &(m->mutex));
	}
	return CAF_ERROR;
}


int
pth_cond_timedwait (pth_cond_t *c, pth_mutex_t *m, const struct timespec *tm) {
	if (c != (pth_cond_t *)NULL) {
		pthread_cond_timedwait (&(c->cond), &(m->mutex), tm);
	}
	return CAF_ERROR;
}

/* caf_thread_cond.c ends here */
