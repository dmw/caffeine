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

#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_deque.h"

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
	test_create ();
	test_new ();
	test_pop ();
	test_get ();
	test_map ();
	return 0;
}


void
test_create (void) {
	deque_t *lst = deque_create ();
	printf ("deque_t *: zero elements list\n");
	printf ("list length: %d\n", deque_length (lst));
	lst = deque_new (strdup("1"));
	deque_push (lst, strdup("2"));
	deque_push (lst, strdup("3"));
	deque_push (lst, strdup("4"));
	deque_push (lst, strdup("5"));
	deque_push (lst, strdup("6"));
	deque_dump (stdout, lst, deque_dump_str_cb);
	printf ("list length: %d\n", deque_length (lst));
	deque_delete (lst, deque_str_delete_cb);
}


void
test_new (void) {
	deque_t *lst = deque_new (strdup("1"));
	printf ("deque_t *: one element list\n");
	deque_dump (stdout, lst, deque_dump_str_cb);
	printf ("list length: %d\n", deque_length (lst));
	deque_delete (lst, deque_str_delete_cb);

	printf ("deque_t *: two elements list\n");
	lst = deque_new (strdup("1"));
	deque_push (lst, strdup("2"));
	deque_dump (stdout, lst, deque_dump_str_cb);
	printf ("list length: %d\n", deque_length (lst));
	deque_delete (lst, deque_str_delete_cb);

	printf ("deque_t *: three elements list\n");
	lst = deque_new (strdup("1"));
	deque_push (lst, strdup("2"));
	deque_push (lst, strdup("3"));
	deque_dump (stdout, lst, deque_dump_str_cb);
	printf ("list length: %d\n", deque_length (lst));
	deque_delete (lst, deque_str_delete_cb);

	printf ("deque_t *: many elements list\n");
	lst = deque_new (strdup("1"));
	deque_push (lst, strdup("2"));
	deque_push (lst, strdup("3"));
	deque_push (lst, strdup("4"));
	deque_push (lst, strdup("5"));
	deque_push (lst, strdup("6"));
	deque_dump (stdout, lst, deque_dump_str_cb);
	printf ("list length: %d\n", deque_length (lst));
	deque_delete (lst, deque_str_delete_cb);
}


void
test_pop (void) {
	deque_t *lst = deque_new (strdup("1"));
	caf_dequen_t *n = (caf_dequen_t *)NULL;
	printf ("deque_t *: many element list\n");
	deque_push (lst, strdup("2"));
	deque_push (lst, strdup("3"));
	deque_push (lst, strdup("4"));
	deque_push (lst, strdup("5"));
	deque_push (lst, strdup("6"));
	deque_dump (stdout, lst, deque_dump_str_cb);
	printf ("deque_t *: after 3 elements pops list\n");
	n = deque_pop (lst);
	if (n != (caf_dequen_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", deque_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = deque_pop (lst);
	if (n != (caf_dequen_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", deque_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = deque_pop (lst);
	if (n != (caf_dequen_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", deque_length (lst));
		xfree (n->data);
		xfree (n);
	}
	deque_dump (stdout, lst, deque_dump_str_cb);
	printf ("list length: %d\n", deque_length (lst));
	deque_delete (lst, deque_str_delete_cb);
}


void
test_get (void) {
	char *rpl1;
	char *rpl2;
	char *rpl3;
	char *rpl4;
	deque_t *lst = deque_new (strdup("1"));
	printf ("deque_t *: many element list\n");
	deque_push (lst, strdup("2"));
	deque_push (lst, strdup("3"));
	deque_push (lst, strdup("4"));
	deque_push (lst, strdup("5"));
	deque_push (lst, strdup("6"));
	deque_dump (stdout, lst, deque_dump_str_cb);
	printf ("list length: %d\n", deque_length (lst));
	rpl1 = (char *)deque_get (lst, 0);
	rpl2 = (char *)deque_get (lst, 1);
	rpl3 = (char *)deque_get (lst, 4);
	rpl4 = (char *)deque_get (lst, 5);
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
	deque_set (lst, 0, rpl1);
	deque_set (lst, 1, rpl2);
	deque_set (lst, 5, rpl4);
	printf ("deque_t *: list after get/set\n");
	deque_dump (stdout, lst, deque_dump_str_cb);
	deque_delete (lst, deque_str_delete_cb);
}


void
test_map (void) {
	char *d1 = strdup("3");
	char *d2 = strdup("4");
	caf_dequen_t *n1;
	caf_dequen_t *n2;
	deque_t *lst = deque_new (strdup("1"));
	printf ("lst %p (many items)\n", (void *)lst);
	deque_push (lst, strdup("2"));
	deque_push (lst, d1);
	deque_push (lst, d2);
	deque_push (lst, strdup("5"));
	deque_push (lst, strdup("6"));
	deque_map (lst, test_map_cb);
	n1 = deque_search_node (lst, d1);
	n2 = deque_search_node (lst, d2);
	printf ("node %p = '%s'\n", (void *)n1, (char *)d1);
	printf ("node %p = '%s'\n", (void *)n2, (char *)d2);
	deque_delete (lst, deque_str_delete_cb);
}


int
test_map_cb (void *data) {
	printf ("item %p = '%s'\n", data, (char *)data);
	return 0;
}

/* caf_deque.c ends here */
