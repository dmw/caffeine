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
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_thread_once.h"


pth_once_t *
pth_once_new (PTH_ONCE_RTN(rtn)) {
	pthread_once_t init = PTHREAD_ONCE_INIT;
	pth_once_t *onc = (pth_once_t *)NULL;
	if (rtn != NULL) {
		onc = (pth_once_t *)xmalloc (PTH_ONCE_SZ);
		if (onc != (pth_once_t *)NULL) {
			onc->once = init;
			onc->rtn = rtn;
		}
	}
	return onc;
}


void
pth_once_delete (pth_once_t *onc) {
	if (onc != (pth_once_t *)NULL) {
		xfree (onc);
	}
}


int
pth_once_call (pth_once_t *call) {
	return pthread_once ((pthread_once_t *)&(call->once), call->rtn);
}

/* caf_thread_once.c ends here */
