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
#include "caf/caf_data_lstdlc.h"

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
	lstdlc_t *lst = lstdlc_create ();
	char *str3 = strdup("3");
	char *str4 = strdup("4");
	printf ("lstdlc_t *: zero elements list\n");
	printf ("list length: %d\n", lstdlc_length (lst));
	lstdlc_push (lst, strdup("2"));
	lstdlc_push (lst, str3);
	lstdlc_push (lst, str4);
	lstdlc_push (lst, strdup("5"));
	lstdlc_push (lst, strdup("6"));
	lstdlc_node_delete_by_data (lst, str3, lstdlc_str_delete_cb);
	lstdlc_node_delete (lst, lstdlc_search_node (lst, str4),
	                    lstdlc_str_delete_cb);
	lstdlc_dump (stdout, lst, lstdlc_dump_str_cb);
	printf ("list length: %d\n", lstdlc_length (lst));
	lstdlc_delete (lst, lstdlc_str_delete_cb);
}


void
test_new (void) {
	lstdlc_t *lst = lstdlc_new (strdup("1"));
	printf ("lstdlc_t *: 1 element list\n");
	lstdlc_dump (stdout, lst, lstdlc_dump_str_cb);
	printf ("list length: %d\n", lstdlc_length (lst));
	lstdlc_delete (lst, lstdlc_str_delete_cb);

	printf ("lstdlc_t *: 2 elements list\n");
	lst = lstdlc_new (strdup("1"));
	lstdlc_push (lst, strdup("2"));
	lstdlc_dump (stdout, lst, lstdlc_dump_str_cb);
	printf ("list length: %d\n", lstdlc_length (lst));
	lstdlc_delete (lst, lstdlc_str_delete_cb);

	printf ("lstdlc_t *: 3 elements list\n");
	lst = lstdlc_new (strdup("1"));
	lstdlc_push (lst, strdup("2"));
	lstdlc_push (lst, strdup("3"));
	lstdlc_dump (stdout, lst, lstdlc_dump_str_cb);
	printf ("list length: %d\n", lstdlc_length (lst));
	lstdlc_delete (lst, lstdlc_str_delete_cb);

	printf ("lstdlc_t *: 6 elements list\n");
	lst = lstdlc_new (strdup("1"));
	lstdlc_push (lst, strdup("2"));
	lstdlc_push (lst, strdup("3"));
	lstdlc_push (lst, strdup("4"));
	lstdlc_push (lst, strdup("5"));
	lstdlc_push (lst, strdup("6"));
	lstdlc_dump (stdout, lst, lstdlc_dump_str_cb);
	printf ("list length: %d\n", lstdlc_length (lst));
	lstdlc_delete (lst, lstdlc_str_delete_cb);
}


void
test_pop (void) {
	lstdlc_t *lst = lstdlc_new (strdup("1"));
	lstdlcn_t *n = (lstdlcn_t *)NULL;
	printf ("lstdlc_t *: many element list\n");
	lstdlc_push (lst, strdup("2"));
	lstdlc_push (lst, strdup("3"));
	lstdlc_push (lst, strdup("4"));
	lstdlc_push (lst, strdup("5"));
	lstdlc_push (lst, strdup("6"));
	lstdlc_dump (stdout, lst, lstdlc_dump_str_cb);
	printf ("lstdlc_t *: after 3 elements pops list\n");
	n = lstdlc_pop (lst);
	if (n != (lstdlcn_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", lstdlc_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = lstdlc_pop (lst);
	if (n != (lstdlcn_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", lstdlc_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = lstdlc_pop (lst);
	if (n != (lstdlcn_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", lstdlc_length (lst));
		xfree (n->data);
		xfree (n);
	}
	lstdlc_dump (stdout, lst, lstdlc_dump_str_cb);
	printf ("list length: %d\n", lstdlc_length (lst));
	lstdlc_delete (lst, lstdlc_str_delete_cb);
}


void
test_get (void) {
	char *rpl1;
	char *rpl2;
	char *rpl3;
	char *rpl4;
	lstdlc_t *lst = lstdlc_new (strdup("1"));
	printf ("lstdlc_t *: many element list\n");
	lstdlc_push (lst, strdup("2"));
	lstdlc_push (lst, strdup("3"));
	lstdlc_push (lst, strdup("4"));
	lstdlc_push (lst, strdup("5"));
	lstdlc_push (lst, strdup("6"));
	lstdlc_dump (stdout, lst, lstdlc_dump_str_cb);
	printf ("list length: %d\n", lstdlc_length (lst));
	rpl1 = (char *)lstdlc_get (lst, 0);
	rpl2 = (char *)lstdlc_get (lst, 1);
	rpl3 = (char *)lstdlc_get (lst, 4);
	rpl4 = (char *)lstdlc_get (lst, 5);
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
	lstdlc_set (lst, 0, rpl1);
	lstdlc_set (lst, 1, rpl2);
	lstdlc_set (lst, 5, rpl4);
	printf ("lstdlc_t *: list after get/set\n");
	lstdlc_dump (stdout, lst, lstdlc_dump_str_cb);
	lstdlc_delete (lst, lstdlc_str_delete_cb);
}


void
test_walk (void) {
	char *d1 = strdup("3");
	char *d2 = strdup("4");
	lstdlcn_t *n1;
	lstdlcn_t *n2;
	lstdlc_t *lst = lstdlc_new (strdup("1"));
	printf ("lst %p (many items)\n", (void *)lst);
	lstdlc_push (lst, strdup("2"));
	lstdlc_push (lst, d1);
	lstdlc_push (lst, d2);
	lstdlc_push (lst, strdup("5"));
	lstdlc_push (lst, strdup("6"));
	lstdlc_walk (lst, test_walk_cb);
	n1 = lstdlc_search_node (lst, d1);
	n2 = lstdlc_search_node (lst, d2);
	printf ("node %p = '%s'\n", (void *)n1, (char *)d1);
	printf ("node %p = '%s'\n", (void *)n2, (char *)d2);
	lstdlc_delete (lst, lstdlc_str_delete_cb);
}


int
test_walk_cb (void *data) {
	printf ("item %p = '%s'\n", data, (char *)data);
	return 0;
}

/* caf_lstdl.c ends here */
