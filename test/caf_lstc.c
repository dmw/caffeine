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

#include "caf/caf.h"
#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstc.h"

#ifdef _GNU_SOURCE
#define strdup(s)           strndup (s, 1024)
#endif /* !_GNU_SOURCE */

void test_create (void);
void test_new (void);
void test_pop (void);
void test_get (void);
void test_map (void);
int test_map_cb (void *data);


int
main (void) {
	test_new ();
	test_pop ();
	test_get ();
	test_map ();
	return 0;
}


void
test_create (void) {
	lstcn_t *lst = lstc_create ();
	char *str2 = strdup("2");
	char *str3 = strdup("3");
	char *str4 = strdup("4");
	printf ("lstc_t *: zero elements list\n");
	lstc_push (lst, str2);
	lstc_push (lst, str3);
	lstc_push (lst, str4);
	lstc_push (lst, strdup("5"));
	lstc_push (lst, strdup("6"));
	printf ("list length: %d\n", lstc_length (lst));
	lstc_dump (stdout, lst, lstc_dump_str_cb);
	lstc_node_delete_by_data (lst, str2, lstc_str_delete_cb);
	lstc_node_delete_by_data (lst, str3, lstc_str_delete_cb);
	lstc_node_delete (lst, lstc_search_node (lst, str4), lstc_str_delete_cb);
	printf ("list length: %d\n", lstc_length (lst));
	lstc_delete (lst, lstc_str_delete_cb);
}


void
test_new (void) {
	lstcn_t *lst = lstc_new (strdup("1"));
	printf ("lstcn_t * %p: one element list\n", (void *)lst);
	lstc_dump (stdout, lst, lstc_dump_str_cb);
	printf ("list length: %d\n", lstc_length (lst));
	lstc_delete (lst, lstc_str_delete_cb);

	printf ("lstcn_t * %p: two elements list\n", (void *)lst);
	lst = lstc_new (strdup("1"));
	lstc_push (lst, strdup("2"));
	lstc_dump (stdout, lst, lstc_dump_str_cb);
	printf ("list length: %d\n", lstc_length (lst));
	lstc_delete (lst, lstc_str_delete_cb);

	printf ("lstcn_t * %p: three elements list\n", (void *)lst);
	lst = lstc_new (strdup("1"));
	lstc_push (lst, strdup("2"));
	lstc_push (lst, strdup("3"));
	lstc_dump (stdout, lst, lstc_dump_str_cb);
	printf ("list length: %d\n", lstc_length (lst));
	lstc_delete (lst, lstc_str_delete_cb);

	printf ("lstcn_t * %p: many elements list\n", (void *)lst);
	lst = lstc_new (strdup("1"));
	lstc_push (lst, strdup("2"));
	lstc_push (lst, strdup("3"));
	lstc_push (lst, strdup("4"));
	lstc_push (lst, strdup("5"));
	lstc_push (lst, strdup("6"));
	lstc_dump (stdout, lst, lstc_dump_str_cb);
	printf ("list length: %d\n", lstc_length (lst));
	lstc_delete (lst, lstc_str_delete_cb);
}


void
test_pop (void) {
	lstcn_t *lst = lstc_new (strdup("1"));
	lstcn_t *n = (lstcn_t *)NULL;
	printf ("lstcn_t * %p: many element list\n", (void *)lst);
	lstc_push (lst, strdup("2"));
	lstc_push (lst, strdup("3"));
	lstc_push (lst, strdup("4"));
	lstc_push (lst, strdup("5"));
	lstc_push (lst, strdup("6"));
	lstc_dump (stdout, lst, lstc_dump_str_cb);
	printf ("lstcn_t *: after 3 elements pops list\n");
	n = lstc_pop (lst);
	if (n != (lstcn_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", lstc_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = lstc_pop (lst);
	if (n != (lstcn_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", lstc_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = lstc_pop (lst);
	if (n != (lstcn_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", lstc_length (lst));
		xfree (n->data);
		xfree (n);
	}
	lstc_dump (stdout, lst, lstc_dump_str_cb);
	printf ("list length: %d\n", lstc_length (lst));
	lstc_delete (lst, lstc_str_delete_cb);
}


void
test_get (void) {
	char *rpl1;
	char *rpl2;
	char *rpl3;
	char *rpl4;
	lstcn_t *lst = lstc_new (strdup("1.1"));
	printf ("sz: %d\n", CAF_LSTC_SZ);
	printf ("lstc_t * %p: many element list\n", (void *)lst);
	lstc_push (lst, strdup("2.1"));
	lstc_push (lst, strdup("3.1"));
	lstc_push (lst, strdup("4.1"));
	lstc_push (lst, strdup("5.1"));
	lstc_push (lst, strdup("6.1"));
	lstc_dump (stdout, lst, lstc_dump_str_cb);
	printf ("list length: %d\n", lstc_length (lst));
	rpl1 = (char *)lstc_get (lst, 0);
	rpl2 = (char *)lstc_get (lst, 1);
	rpl3 = (char *)lstc_get (lst, 4);
	rpl4 = (char *)lstc_get (lst, 5);
	printf ("element 1: %s\n", (char *)rpl1);
	printf ("element 2: %s\n", (char *)rpl2);
	printf ("element 5: %s\n", (char *)rpl3);
	printf ("element 6: %s\n", (char *)rpl4);
	xfree (rpl1);
	xfree (rpl2);
	xfree (rpl4);
	rpl1 = strdup ("x.1");
	rpl2 = strdup ("y.2");
	rpl4 = strdup ("z.3");
	lstc_set (lst, 0, rpl1);
	lstc_set (lst, 1, rpl2);
	lstc_set (lst, 5, rpl4);
	printf ("lstc_t * %p: list after get/set\n", (void *)lst);
	lstc_dump (stdout, lst, lstc_dump_str_cb);
	printf ("lstc_t * %p: dumped list\n", (void *)lst);
	lstc_delete (lst, lstc_str_delete_cb);
}


void
test_map (void) {
	char *d1;
	char *d2;
	char *d3;
	lstcn_t *n1;
	lstcn_t *n2;
	lstcn_t *lst = lstc_new (strdup("1.1"));
	d3 = strdup("2.1");
	d1 = strdup("3.1");
	d2 = strdup("4.1");
	printf ("sz: %d\n", CAF_LSTC_SZ);
	printf ("lst %p (many items)\n", (void *)lst);
	lstc_push (lst, d3);
	lstc_push (lst, d1);
	lstc_push (lst, d2);
	lstc_push (lst, strdup("5.1"));
	lstc_push (lst, strdup("6.1"));
	lstc_map (lst, test_map_cb);
	n1 = lstc_search_node (lst, d1);
	n2 = lstc_search_node (lst, d2);
	printf ("node %p = '%s'\n", (void *)n1, (char *)d1);
	printf ("node %p = '%s'\n", (void *)n2, (char *)d2);
	lstc_delete (lst, lstc_str_delete_cb);
}


int
test_map_cb (void *data) {
	printf ("item %p = '%s'\n", data, (char *)data);
	return CAF_OK;
}

/* caf_lstc.c ends here */
