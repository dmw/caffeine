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
#include "caf/caf_data_cdeque.h"

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
	cdeque_t *lst = cdeque_create ();
	char *str3 = strdup("3");
	char *str4 = strdup("4");
	printf ("cdeque_t *: zero elements list\n");
	printf ("list length: %d\n", cdeque_length (lst));
	cdeque_push (lst, strdup("2"));
	cdeque_push (lst, str3);
	cdeque_push (lst, str4);
	cdeque_push (lst, strdup("5"));
	cdeque_push (lst, strdup("6"));
	cdeque_node_delete_by_data (lst, str3, cdeque_str_delete_cb);
	cdeque_node_delete (lst, cdeque_search_node (lst, str4),
	                    cdeque_str_delete_cb);
	cdeque_dump (stdout, lst, cdeque_dump_str_cb);
	printf ("list length: %d\n", cdeque_length (lst));
	cdeque_delete (lst, cdeque_str_delete_cb);
}


void
test_new (void) {
	cdeque_t *lst = cdeque_new (strdup("1"));
	printf ("cdeque_t *: 1 element list\n");
	cdeque_dump (stdout, lst, cdeque_dump_str_cb);
	printf ("list length: %d\n", cdeque_length (lst));
	cdeque_delete (lst, cdeque_str_delete_cb);

	printf ("cdeque_t *: 2 elements list\n");
	lst = cdeque_new (strdup("1"));
	cdeque_push (lst, strdup("2"));
	cdeque_dump (stdout, lst, cdeque_dump_str_cb);
	printf ("list length: %d\n", cdeque_length (lst));
	cdeque_delete (lst, cdeque_str_delete_cb);

	printf ("cdeque_t *: 3 elements list\n");
	lst = cdeque_new (strdup("1"));
	cdeque_push (lst, strdup("2"));
	cdeque_push (lst, strdup("3"));
	cdeque_dump (stdout, lst, cdeque_dump_str_cb);
	printf ("list length: %d\n", cdeque_length (lst));
	cdeque_delete (lst, cdeque_str_delete_cb);

	printf ("cdeque_t *: 6 elements list\n");
	lst = cdeque_new (strdup("1"));
	cdeque_push (lst, strdup("2"));
	cdeque_push (lst, strdup("3"));
	cdeque_push (lst, strdup("4"));
	cdeque_push (lst, strdup("5"));
	cdeque_push (lst, strdup("6"));
	cdeque_dump (stdout, lst, cdeque_dump_str_cb);
	printf ("list length: %d\n", cdeque_length (lst));
	cdeque_delete (lst, cdeque_str_delete_cb);
}


void
test_pop (void) {
	cdeque_t *lst = cdeque_new (strdup("1"));
	caf_cdequen_t *n = (caf_cdequen_t *)NULL;
	printf ("cdeque_t *: many element list\n");
	cdeque_push (lst, strdup("2"));
	cdeque_push (lst, strdup("3"));
	cdeque_push (lst, strdup("4"));
	cdeque_push (lst, strdup("5"));
	cdeque_push (lst, strdup("6"));
	cdeque_dump (stdout, lst, cdeque_dump_str_cb);
	printf ("cdeque_t *: after 3 elements pops list\n");
	n = cdeque_pop (lst);
	if (n != (caf_cdequen_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", cdeque_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = cdeque_pop (lst);
	if (n != (caf_cdequen_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", cdeque_length (lst));
		xfree (n->data);
		xfree (n);
	}
	n = cdeque_pop (lst);
	if (n != (caf_cdequen_t *)NULL) {
		printf ("pops: %s\n", (char *)n->data);
		printf ("list length: %d\n", cdeque_length (lst));
		xfree (n->data);
		xfree (n);
	}
	cdeque_dump (stdout, lst, cdeque_dump_str_cb);
	printf ("list length: %d\n", cdeque_length (lst));
	cdeque_delete (lst, cdeque_str_delete_cb);
}


void
test_get (void) {
	char *rpl1;
	char *rpl2;
	char *rpl3;
	char *rpl4;
	cdeque_t *lst = cdeque_new (strdup("1"));
	printf ("cdeque_t *: many element list\n");
	cdeque_push (lst, strdup("2"));
	cdeque_push (lst, strdup("3"));
	cdeque_push (lst, strdup("4"));
	cdeque_push (lst, strdup("5"));
	cdeque_push (lst, strdup("6"));
	cdeque_dump (stdout, lst, cdeque_dump_str_cb);
	printf ("list length: %d\n", cdeque_length (lst));
	rpl1 = (char *)cdeque_get (lst, 0);
	rpl2 = (char *)cdeque_get (lst, 1);
	rpl3 = (char *)cdeque_get (lst, 4);
	rpl4 = (char *)cdeque_get (lst, 5);
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
	cdeque_set (lst, 0, rpl1);
	cdeque_set (lst, 1, rpl2);
	cdeque_set (lst, 5, rpl4);
	printf ("cdeque_t *: list after get/set\n");
	cdeque_dump (stdout, lst, cdeque_dump_str_cb);
	cdeque_delete (lst, cdeque_str_delete_cb);
}


void
test_map (void) {
	char *d1 = strdup("3");
	char *d2 = strdup("4");
	caf_cdequen_t *n1;
	caf_cdequen_t *n2;
	cdeque_t *lst = cdeque_new (strdup("1"));
	printf ("lst %p (many items)\n", (void *)lst);
	cdeque_push (lst, strdup("2"));
	cdeque_push (lst, d1);
	cdeque_push (lst, d2);
	cdeque_push (lst, strdup("5"));
	cdeque_push (lst, strdup("6"));
	cdeque_map (lst, test_map_cb);
	n1 = cdeque_search_node (lst, d1);
	n2 = cdeque_search_node (lst, d2);
	printf ("node %p = '%s'\n", (void *)n1, (char *)d1);
	printf ("node %p = '%s'\n", (void *)n2, (char *)d2);
	cdeque_delete (lst, cdeque_str_delete_cb);
}


int
test_map_cb (void *data) {
	printf ("item %p = '%s'\n", data, (char *)data);
	return 0;
}

/* caf_cdeque.c ends here */
