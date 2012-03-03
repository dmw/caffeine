/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "caf" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
/*
  Caffeine - C Application Framework
  Copyright (C) 2006 Daniel Molina Wegener <dmw@coder.cl>

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
#include "caf/caf_thread_attr.h"


pth_attri_t *
pth_attri_new (void) {
	pth_attri_t *attri = (pth_attri_t *)NULL;
	attri = (pth_attri_t *)xmalloc (CAF_PT_ATTRI_SZ);
	if (attri != (pth_attri_t *)NULL) {
		attri->at = 0;
	}
	return attri;
}


void
pth_attri_delete (pth_attri_t *attri) {
	if (attri != (pth_attri_t *)NULL) {
		xfree (attri);
	}
}


int
pth_attr_init (pth_attri_t *attri) {
	if (attri != (pth_attri_t *)NULL) {
		return pthread_attr_init(&(attri->attr));
	}
	return CAF_ERROR_SUB;
}


int
pth_attr_destroy (pth_attri_t *attri) {
	if (attri != (pth_attri_t *)NULL) {
		return pthread_attr_destroy(&(attri->attr));
	}
	return CAF_ERROR_SUB;
}


pth_attri_t *
pth_attri_init (void) {
	int rt = -1;
	pth_attri_t *attri = pth_attri_new ();
	if (attri != (pth_attri_t *)NULL) {
		rt = pth_attr_init (attri);
		if (rt != 0) {
			pth_attri_delete (attri);
		} else {
			return attri;
		}
	}
	return attri;
}


int
pth_attri_destroy (pth_attri_t *attri) {
	int rt = -1;
	if (attri != (pth_attri_t *)NULL) {
		rt = pth_attr_destroy(attri);
		if (rt == 0) {
			pth_attri_delete (attri);
			return CAF_OK;
		}
	}
	return CAF_ERROR_SUB;
}


int
pth_attri_set (pth_attri_t *attri, pth_attr_types_t t, void *data) {
	int *di = (int *)data;
	size_t *ds = (size_t *)data;
	if (attri != (pth_attri_t *)NULL) {
		switch (t) {
			/* pthread_attr_setdetachstate */
		case PTH_ATTR_DETACHED:
			attri->at |= t;
			return pthread_attr_setdetachstate (&(attri->attr),
			                                    PTHREAD_CREATE_JOINABLE);
			/* pthread_attr_setdetachstate */
		case PTH_ATTR_JOINABLE:
			attri->at |= t;
			return pthread_attr_setdetachstate (&(attri->attr),
			                                    PTHREAD_CREATE_DETACHED);
#ifndef LINUX
			/* pthread_attr_setstacksize */
		case PTH_ATTR_STACKSZ:
			attri->at |= t;
			return pthread_attr_setstacksize (&(attri->attr), *ds);
			/* pthread_attr_setguardsize */
		case PTH_ATTR_GUARDSZ:
			attri->at |= t;
			return pthread_attr_setguardsize (&(attri->attr), *ds);
#endif /* !LINUX */
			/* pthread_attr_setschedparam */
		case PTH_ATTR_SCHEDPARAM:
			attri->at |= t;
			return pthread_attr_setschedparam (&(attri->attr),
			                                   (struct sched_param *)data);
			/* pthread_attr_setinheritsched */
		case PTH_ATTR_INSCHEDPARAM:
			attri->at |= t;
			return pthread_attr_setinheritsched (&(attri->attr), *di);
			/* pthread_attr_setschedpolicy */
		case PTH_ATTR_SCHEDPOLICY:
			attri->at |= t;
			return pthread_attr_setschedpolicy (&(attri->attr), *di);
			/* pthread_attr_setscope */
		case PTH_ATTR_SCOPE:
			attri->at |= t;
			return pthread_attr_setscope (&(attri->attr), *di);
		default:
			return CAF_ERROR_SUB;
		}
	}
	return CAF_ERROR_SUB;
}


int
pth_attri_get (pth_attri_t *attri, pth_attr_types_t t, void *data) {
	if (attri != (pth_attri_t *)NULL) {
		switch (t) {
			/* pthread_attr_getdetachstate */
		case PTH_ATTR_DETACHED:
			return pthread_attr_getdetachstate (&(attri->attr), (int *)data);
			/* pthread_attr_getdetachstate */
		case PTH_ATTR_JOINABLE:
			return pthread_attr_getdetachstate (&(attri->attr), (int *)data);
#ifndef LINUX
			/* pthread_attr_getstacksize */
		case PTH_ATTR_STACKSZ:
			return pthread_attr_getstacksize (&(attri->attr), (size_t *)data);
			/* pthread_attr_getguardsize */
		case PTH_ATTR_GUARDSZ:
			return pthread_attr_getguardsize (&(attri->attr), (size_t *)data);
#endif /* !LINUX */
			/* pthread_attr_getschedparam */
		case PTH_ATTR_SCHEDPARAM:
			return pthread_attr_getschedparam (&(attri->attr),
			                                   (struct sched_param *)data);
			/* pthread_attr_getinheritsched */
		case PTH_ATTR_INSCHEDPARAM:
			return pthread_attr_getinheritsched (&(attri->attr), (int *)data);
			/* pthread_attr_getschedpolicy */
		case PTH_ATTR_SCHEDPOLICY:
			return pthread_attr_getschedpolicy (&(attri->attr), (int *)data);
			/* pthread_attr_getscope */
		case PTH_ATTR_SCOPE:
			return pthread_attr_getscope (&(attri->attr), (int *)data);
		default:
			return CAF_ERROR_SUB;
		}
	}
	return CAF_ERROR_SUB;
}

/* caf_thread_attr.c ends here */

