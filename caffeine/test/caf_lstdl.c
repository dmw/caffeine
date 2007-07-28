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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstdl.h"

#ifdef _GNU_SOURCE
#define strdup(s)           strndup (s, 1024)
#endif /* !_GNU_SOURCE */

void test_create (void);
void test_new (void);
void test_pop (void);
void test_get (void);
void test_walk (void);
int test_walk_cb (void *data);


int
main (void) {
	test_create ();
	test_new ();
	test_pop ();
	test_get ();
	test_walk ();
	return 0;
}


void
test_create (void) {
	lstdl_t *lst = lstdl_create ();
	printf ("lstdl_t *: zero elements list\n");
	printf ("list length: %d\n", lstdl_length (lst));
	lst = lstdl_new (strdup("1"));
	lstdl_push (lst, strdup("2"));
	lstdl_push (lst, strdup("3"));
	lstdl_push (lst, strdup("4"));
	lstdl_push (lst, strdup("5"));
	lstdl_push (lst, strdup("6"));
	lstdl_dump (stdout, lst, lstdl_dump_str_cb);
	printf ("list length: %d\n", lstdl_length (lst));
	lstdl_delete (lst, lstdl_str_delete_cb);
}


void
test_new (void) {
	lstdl_t *lst = lstdl_new (strdup("1"));
	printf ("lstdl_t *: one element list\n");
	lstdl_dump (stdout, lst, lstdl_dump_str_cb);
	printf ("list length: %d\n", lstdl_length (lst));
	lstdl_delete (lst, lstdl_str_delete_cb);

	printf ("lstdl_t *: two elements list\n");
	lst = lstdl_new (strdup("1"));
	lstdl_push (lst, strdup("2"));
	lstdl_dump (stdout, lst, lstdl_dump_str_cb);
	printf ("list length: %d\n", lstdl_length (lst));
	lstdl_delete (lst, lstdl_str_delete_cb);

	printf ("lstdl_t *: three elements list\n");
	lst = lstdl_new (strdup("1"));
	lstdl_push (lst, strdup("2"));
	lstdl_push (lst, strdup("3"));
	lstdl_dump (stdout, lst, lstdl_dump_str_cb);
	printf ("list length: %d\n", lstdl_length (lst));
	lstdl_delete (lst, lstdl_str_delete_cb);

	printf ("lstdl_t *: many elements list\n");
	lst = lstdl_new (strdup("1"));
	lstdl_push (lst, strdup("2"));
	lstdl_push (lst, strdup("3"));
	lstdl_push (lst, strdup("4"));
	lstdl_push (lst, strdup("5"));
	lstdl_push (lst, strdup("6"));
	lstdl_dump (stdout, lst, lstdl_dump_str_cb);
	printf ("list length: %d\n", lstdl_length (lst));
	lstdl_delete (lst, lstdl_str_delete_cb);
}


void
test_pop (void) {
	lstdl_t *lst = lstdl_new (strdup("1"));
	lstdln_t *n = (lstdln_t *)NULL;
	printf ("lstdl_t *: many element list\n");
	lstdl_push (lst, strdup("2"));
	lstdl_push (lst, strdup("3"));
	lstdl_push (lst, strdup("4"));
	lstdl_push (lst, strdup("5"));
	lstdl_push (lst, strdup("6"));
	lstdl_dump (stdout, lst, lstdl_dump_str_cb);
	printf ("lstdl_t *: after 3 elements pops list\n");
	n = lstdl_pop (lst);
	if (n != (lstdln_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", lstdl_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = lstdl_pop (lst);
	if (n != (lstdln_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", lstdl_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = lstdl_pop (lst);
	if (n != (lstdln_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", lstdl_length (lst));
		xfree (n->data);
		xfree (n);
	}
	lstdl_dump (stdout, lst, lstdl_dump_str_cb);
	printf ("list length: %d\n", lstdl_length (lst));
	lstdl_delete (lst, lstdl_str_delete_cb);
}


void
test_get (void) {
	char *rpl1;
	char *rpl2;
	char *rpl3;
	char *rpl4;
	lstdl_t *lst = lstdl_new (strdup("1"));
	printf ("lstdl_t *: many element list\n");
	lstdl_push (lst, strdup("2"));
	lstdl_push (lst, strdup("3"));
	lstdl_push (lst, strdup("4"));
	lstdl_push (lst, strdup("5"));
	lstdl_push (lst, strdup("6"));
	lstdl_dump (stdout, lst, lstdl_dump_str_cb);
	printf ("list length: %d\n", lstdl_length (lst));
	rpl1 = (char *)lstdl_get (lst, 0);
	rpl2 = (char *)lstdl_get (lst, 1);
	rpl3 = (char *)lstdl_get (lst, 4);
	rpl4 = (char *)lstdl_get (lst, 5);
	printf ("element 1: %s\n", (char *)rpl1);
	printf ("element 2: %s\n", (char *)rpl2);
	printf ("element 5: %s\n", (char *)rpl3);
	printf ("element 6: %s\n", (char *)rpl4);
	xfree (rpl1);
	xfree (rpl2);
	xfree (rpl4);
	rpl1 = strdup ("x");
	rpl2 = strdup ("y");
	rpl4 = strdup ("z");
	lstdl_set (lst, 0, rpl1);
	lstdl_set (lst, 1, rpl2);
	lstdl_set (lst, 5, rpl4);
	printf ("lstdl_t *: list after get/set\n");
	lstdl_dump (stdout, lst, lstdl_dump_str_cb);
	lstdl_delete (lst, lstdl_str_delete_cb);
}


void
test_walk (void) {
	char *d1 = strdup("3");
	char *d2 = strdup("4");
	lstdln_t *n1;
	lstdln_t *n2;
	lstdl_t *lst = lstdl_new (strdup("1"));
	printf ("lst %p (many items)\n", (void *)lst);
	lstdl_push (lst, strdup("2"));
	lstdl_push (lst, d1);
	lstdl_push (lst, d2);
	lstdl_push (lst, strdup("5"));
	lstdl_push (lst, strdup("6"));
	lstdl_walk (lst, test_walk_cb);
	n1 = lstdl_search_node (lst, d1);
	n2 = lstdl_search_node (lst, d2);
	printf ("node %p = '%s'\n", (void *)n1, (char *)d1);
	printf ("node %p = '%s'\n", (void *)n2, (char *)d2);
	lstdl_delete (lst, lstdl_str_delete_cb);
}


int
test_walk_cb (void *data) {
	printf ("item %p = '%s'\n", data, (char *)data);
	return 0;
}

/* caf_lstdl.c ends here */
