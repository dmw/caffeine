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
#include "caf/caf_thread_key.h"
#include "caf/caf_thread_once.h"

#define KP1_ID      1
#define KP2_ID      2
#define KP3_ID      3

#define K1_ID       1
#define K2_ID       2
#define K3_ID       3

void *pth_rtn_1 (void *p);
void *pth_rtn_2 (void *p);
void *pth_rtn_3 (void *p);

pth_once_t *o1;
pth_once_t *o2;
pth_once_t *o3;

pth_kpool_t *kp1;
pth_kpool_t *kp2;
pth_kpool_t *kp3;

pthread_t thr1;
pthread_t thr2;
pthread_t thr3;

int thr1_run = 1;
int thr2_run = 1;
int thr3_run = 1;

const char str1[] = "hi";
const char str2[] = "die";
const char str3[] = "bye";

size_t len1 = 0;
size_t len2 = 0;
size_t len3 = 0;

void create_keys_1 (void);
void create_keys_2 (void);
void create_keys_3 (void);

void *pth_rtn1 (void *p);
void *pth_rtn2 (void *p);
void *pth_rtn3 (void *p);

void free_ptr (void *p);

int
main (void) {
	int rt = 0;
	pth_attri_t *attr = (pth_attri_t *)NULL;
	attr = pth_attri_new ();
	len1 = strlen (str1) + 1;
	len2 = strlen (str2) + 1;
	len3 = strlen (str3) + 1;
	if (attr != (pth_attri_t *)NULL) {
		rt = pth_attr_init (attr);
		if (rt == 0) {
			rt = pth_attri_set (attr, PTH_ATTR_DETACHED, (void *)NULL);
			pthread_create (&thr1, &(attr->attr), pth_rtn1, (void *)NULL);
			pthread_create (&thr2, &(attr->attr), pth_rtn2, (void *)NULL);
			pthread_create (&thr3, &(attr->attr), pth_rtn3, (void *)NULL);
			sleep (15);
			thr1_run = 0;
			thr2_run = 0;
			thr3_run = 0;
			sleep (5);
		}
	}
	pth_kpool_delete (kp1);
	pth_kpool_delete (kp2);
	pth_kpool_delete (kp3);
	pth_attri_delete (attr);
	printf ("done\n");
	pthread_exit (NULL);
	return 0;
}

void
create_keys_1 (void) {
	void *ptr1 = (void *)NULL, *ptr2 = (void *)NULL, *ptr3 = (void *)NULL;
	kp1 = pth_kpool_new (KP1_ID, &thr1);
	if (kp1 != (pth_kpool_t *)NULL) {
		pth_kpool_add (kp1, pth_key_new (K1_ID, free_ptr));
		pth_kpool_add (kp1, pth_key_new (K2_ID, free_ptr));
		pth_kpool_add (kp1, pth_key_new (K3_ID, free_ptr));

		pth_key_set (pth_kpool_get_key (kp1, K1_ID), len1);
		pth_key_set (pth_kpool_get_key (kp1, K2_ID), len2);
		pth_key_set (pth_kpool_get_key (kp1, K3_ID), len3);

		ptr1 = pth_kpool_get (kp1, K1_ID);
		ptr2 = pth_kpool_get (kp1, K2_ID);
		ptr3 = pth_kpool_get (kp1, K3_ID);

		if (ptr1 != (void *)NULL) {
			memset (ptr1, (int)NULL, len1);
			memcpy (ptr1, str1, len1);
		}
		if (ptr2 != (void *)NULL) {
			memset (ptr2, (int)NULL, len2);
			memcpy (ptr2, str2, len1);
		}
		if (ptr3 != (void *)NULL) {
			memset (ptr3, (int)NULL, len3);
			memcpy (ptr3, str3, len3);
		}
	}
}

void
create_keys_2 (void) {
	void *ptr1 = (void *)NULL, *ptr2 = (void *)NULL, *ptr3 = (void *)NULL;
	kp2 = pth_kpool_new (KP2_ID, &thr2);
	if (kp2 != (pth_kpool_t *)NULL) {
		pth_kpool_add (kp2, pth_key_new (K1_ID, free_ptr));
		pth_kpool_add (kp2, pth_key_new (K2_ID, free_ptr));
		pth_kpool_add (kp2, pth_key_new (K3_ID, free_ptr));

		pth_key_set (pth_kpool_get_key (kp2, K1_ID), len1);
		pth_key_set (pth_kpool_get_key (kp2, K2_ID), len2);
		pth_key_set (pth_kpool_get_key (kp2, K3_ID), len3);

		ptr1 = pth_kpool_get (kp2, K1_ID);
		ptr2 = pth_kpool_get (kp2, K2_ID);
		ptr3 = pth_kpool_get (kp2, K3_ID);

		if (ptr1 != (void *)NULL) {
			memset (ptr1, (int)NULL, len1);
			memcpy (ptr1, str1, len1);
		}
		if (ptr2 != (void *)NULL) {
			memset (ptr2, (int)NULL, len2);
			memcpy (ptr2, str2, len1);
		}
		if (ptr3 != (void *)NULL) {
			memset (ptr3, (int)NULL, len3);
			memcpy (ptr3, str3, len3);
		}
	}
}

void
create_keys_3 (void) {
	void *ptr1 = (void *)NULL, *ptr2 = (void *)NULL, *ptr3 = (void *)NULL;
	kp3 = pth_kpool_new (KP3_ID, &thr3);
	if (kp3 != (pth_kpool_t *)NULL) {
		pth_kpool_add (kp3, pth_key_new (K1_ID, free_ptr));
		pth_kpool_add (kp3, pth_key_new (K2_ID, free_ptr));
		pth_kpool_add (kp3, pth_key_new (K3_ID, free_ptr));

		pth_key_set (pth_kpool_get_key (kp3, K1_ID), len1);
		pth_key_set (pth_kpool_get_key (kp3, K2_ID), len2);
		pth_key_set (pth_kpool_get_key (kp3, K3_ID), len3);

		ptr1 = pth_kpool_get (kp3, K1_ID);
		ptr2 = pth_kpool_get (kp3, K2_ID);
		ptr3 = pth_kpool_get (kp3, K3_ID);

		if (ptr1 != (void *)NULL) {
			memset (ptr1, (int)NULL, len1);
			memcpy (ptr1, str1, len1);
		}
		if (ptr2 != (void *)NULL) {
			memset (ptr2, (int)NULL, len2);
			memcpy (ptr2, str2, len1);
		}
		if (ptr3 != (void *)NULL) {
			memset (ptr3, (int)NULL, len3);
			memcpy (ptr3, str3, len3);
		}
	}
}

void *
pth_rtn1 (void *p) {
	char *s1, *s2, *s3;
	o1 = pth_once_new (create_keys_1);
	pth_once_call (o1);
	printf ("args: %p\n", p);
	s1 = (char *)pth_kpool_get (kp1, K1_ID);
	s2 = (char *)pth_kpool_get (kp1, K2_ID);
	s3 = (char *)pth_kpool_get (kp1, K3_ID);
	while (thr1_run) {
		sleep (1);
		printf ("1. s1: [%d] '%s'\n", K1_ID, s1);
		printf ("1. s2: [%d] '%s'\n", K2_ID, s2);
		printf ("1. s3: [%d] '%s'\n", K3_ID, s3);
	}
	pth_once_delete (o1);
	pthread_exit (NULL);
}

void *
pth_rtn2 (void *p) {
	char *s1, *s2, *s3;
	o2 = pth_once_new (create_keys_2);
	pth_once_call (o2);
	printf ("args: %p\n", p);
	s1 = (char *)pth_kpool_get (kp2, K1_ID);
	s2 = (char *)pth_kpool_get (kp2, K2_ID);
	s3 = (char *)pth_kpool_get (kp2, K3_ID);
	while (thr2_run) {
		sleep (1);
		printf ("2. s1: [%d] '%s'\n", K1_ID, s1);
		printf ("2. s2: [%d] '%s'\n", K2_ID, s2);
		printf ("2. s3: [%d] '%s'\n", K3_ID, s3);
	}
	pth_once_delete (o2);
	pthread_exit (NULL);
}

void *
pth_rtn3 (void *p) {
	char *s1, *s2, *s3;
	o3 = pth_once_new (create_keys_3);
	pth_once_call (o3);
	printf ("args: %p\n", p);
	s1 = (char *)pth_kpool_get (kp3, K1_ID);
	s2 = (char *)pth_kpool_get (kp3, K2_ID);
	s3 = (char *)pth_kpool_get (kp3, K3_ID);
	while (thr3_run) {
		sleep (1);
		printf ("3. s1: [%d] '%s'\n", K1_ID, s1);
		printf ("3. s2: [%d] '%s'\n", K2_ID, s2);
		printf ("3. s3: [%d] '%s'\n", K3_ID, s3);
	}
	pth_once_delete (o3);
	pthread_exit (NULL);
}

void
free_ptr (void *p) {
	printf ("free: %p\n", p);
	xfree (p);
}

/* caf_pth_key.c ends here */
