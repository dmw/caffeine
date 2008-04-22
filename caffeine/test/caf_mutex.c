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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_pidfile.h"
#include "caf/caf_thread_attr.h"
#include "caf/caf_thread_pool.h"
#include "caf/caf_thread_mutex.h"


pth_mutex_t *common_mutex;

void *pth_rtn(void *p);
int mutex_init (void);
int mutex_destroy (void);
int mutex_lock (void);
int mutex_unlock (void);

int all_done = 1;

int
main (void) {
	int rt = 0, cnt = 0;
	pth_attri_t *attr = (pth_attri_t *)NULL;
	pth_pool_t *pth_pool = (pth_pool_t *)NULL;
	attr = pth_attri_new ();
	if (attr != (pth_attri_t *)NULL) {
		rt = pth_attr_init (attr);
		if (rt == 0) {
			rt = pth_attri_set (attr, PTH_ATTR_JOINABLE, (void *)NULL);
			if (rt == 0) {
				if ((mutex_init ()) == 0) {
					pth_pool = pth_pool_create (attr, pth_rtn, 3, (void *)NULL);
					printf ("%d: threads: %d\n", getpid (),
					        lstdl_length (pth_pool->threads));
					for (cnt = 0; cnt < 3; cnt++) {
						printf ("join: %d\n", cnt);
						pth_pool_join (pth_pool);
						sleep (3);
					}
					all_done = 0;
					sleep (10);
					printf ("pth_pool_cancel\n");
					pth_pool_cancel (pth_pool);
				}
			}
		}
	}
	sleep (2);
	printf ("pth_pool_delete\n");
	pth_pool_delete (pth_pool);
	sleep (2);
	printf ("pth_attri_destroy\n");
	pth_attri_destroy (attr);
	sleep (2);
	printf ("mutex_destroy\n");
	mutex_destroy ();
	sleep (2);
	pthread_exit (NULL);
}

void *
pth_rtn (void *p) {
	pthread_t pt = pthread_self ();
	while (all_done) {
		printf ("lock: %d\n", mutex_lock ());
		printf ("lock status: %d\n",
		        pth_mtxattr_get (common_mutex, PTHDR_MUTEX_LOCK, 0));
		printf ("%d: pthread_self = %p (arg:%p)\n", getpid (), (void *)&pt,
		        p);
		sleep (2);
		printf ("unlock: %d\n", mutex_unlock ());
		printf ("lock status: %d\n",
		        pth_mtxattr_get (common_mutex, PTHDR_MUTEX_LOCK, 0));
	}
	pthread_exit (NULL);
}

int
mutex_init (void) {
	common_mutex = pth_mtx_new ();
	if (common_mutex != (pth_mutex_t *)NULL) {
		if ((pth_mtxattr_init (common_mutex)) == 0) {
			return pth_mtx_init (common_mutex);
		}
	}
	return 1;
}

int
mutex_destroy (void) {
	if ((pth_mtxattr_destroy (common_mutex)) == 0) {
		if ((pth_mtx_destroy (common_mutex)) == 0) {
			pth_mtx_delete (common_mutex);
			return 0;
		}
	}
	return 1;
}

int
mutex_lock (void) {
	return pth_mtx_lock (common_mutex);
}

int
mutex_unlock (void) {
	return pth_mtx_unlock (common_mutex);
}

/* caf_mutex.c ends here */
