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
#include "caf/caf_data_buffer.h"

void test_new (void);
void test_import (void);
void test_split (void);
void test_replace (void);
void test_copy (void);
void test_cut (void);
void test_search (void);
void test_tail_head (void);


int
main (void)
{

    test_new ();
    test_import ();
    test_split ();
    test_replace ();
    test_copy ();
    test_cut ();
    test_search ();
    test_tail_head ();
    return 0;
}

void
test_new (void)
{
    cbuffer_t *buf = cbuf_new ();
    cbuf_delete (buf);
}

void
test_import (void)
{
    char str1[] = "buffer one\n\x01\x02\nend buffer\n";
    char str2[] = "buffer two\n\x01\x02\nend buffer\n";
    char str3[] = "x" "\x01" "y" "\x01" "z" "\x01" "xy" "\x01" "yz";

    cbuffer_t *buf1 = (cbuffer_t *)NULL;
    cbuffer_t *buf2 = (cbuffer_t *)NULL;
    cbuffer_t *buf3 = (cbuffer_t *)NULL;

    buf1 = cbuf_new ();
    buf2 = cbuf_new ();
    buf3 = cbuf_new ();

    cbuf_import (buf1, str1, strlen (str1));
    cbuf_import (buf2, str2, strlen (str2));
    cbuf_import (buf3, str3, strlen (str3));

    cbuf_delete (buf1);
    cbuf_delete (buf2);
    cbuf_delete (buf3);
}

void
test_split (void)
{
    char str0[] = "\x01";
    char str1[] = "\x01\x01x\x01y\x01z\x01xy\x01yz";
    char str2[] = "buffer one\n\x01\x02\nend buffer\n";
    char str3[] = "buffer two\n\x01\x02\nend buffer\n";

    lstdl_t *split = (lstdl_t *)NULL;

    cbuffer_t *buf1 = (cbuffer_t *)NULL;
    cbuffer_t *buf2 = (cbuffer_t *)NULL;
    cbuffer_t *buf3 = (cbuffer_t *)NULL;

    buf1 = cbuf_new ();
    buf2 = cbuf_new ();
    buf3 = cbuf_new ();

    cbuf_import (buf1, str1, strlen (str1));
    cbuf_import (buf2, str2, strlen (str2));
    cbuf_import (buf3, str3, strlen (str3));

    split = cbuf_split (buf1, (void *)str0, 1);
    printf ("test_split(): len = %d\n", lstdl_length (split));

    lstdl_push (split, buf2);
    printf ("test_split(): len = %d\n", lstdl_length (split));

    lstdl_push (split, buf3);
    printf ("test_split(): len = %d\n", lstdl_length (split));

    cbuf_delete (buf1);
    lstdl_delete (split, cbuf_delete_callback);
}

void
test_replace (void)
{
    char str1[] = "\x01\x01\x01x\x01y\x01z\x01xy\x01yz\x01";
    char *str2 = (char *)NULL;

    cbuffer_t *buf1 = (cbuffer_t *)NULL;
    cbuffer_t *buf2 = (cbuffer_t *)NULL;

    buf1 = cbuf_new ();

    cbuf_import (buf1, str1, strlen (str1));

    buf2 = cbuf_replace (buf1, "\x01", ", ", 1, 2);

    if (buf2 != (cbuffer_t *)NULL) {
        str2 = (char *)xmalloc (buf2->sz + 1);
        if (str2 != (char *)NULL) {
            memset (str2, (int)NULL, buf2->sz + 1);
            memcpy (str2, buf2->data, buf2->sz);
            printf ("test_replace(): str1 = %s\n", str1);
            printf ("test_replace(): str2 = %s\n", str2);
            xfree (str2);
        }
    }

    cbuf_delete (buf1);
    cbuf_delete (buf2);
}

void
test_copy (void)
{
    char str1[] = "\x01\x01x\x01\x01y\x01\x01xy\x01\x01";
    char *str2 = (char *)NULL;
    char *str3 = (char *)NULL;

    cbuffer_t *buf1 = (cbuffer_t *)NULL;
    cbuffer_t *buf2 = (cbuffer_t *)NULL;

    buf1 = cbuf_new();
    buf2 = cbuf_new();

    cbuf_import (buf1, str1, strlen (str1));
    cbuf_copy (buf2, buf1);

    printf ("test_copy(): str1 = %s\n", str1);

    str2 = (char *)xmalloc (buf1->sz + 1);
    memset (str2, (int)NULL, buf1->sz + 1);
    memcpy (str2, buf1->data, buf1->sz);

    str3 = (char *)xmalloc (buf2->sz + 1);
    memset (str3, (int)NULL, buf2->sz + 1);
    memcpy (str3, buf2->data, buf2->sz);

    printf ("test_copy(): str2 = %s\ntest_copy(): str3 = %s\n", str2, str3);

    xfree (str2);
    xfree (str3);

    cbuf_delete (buf1);
    cbuf_delete (buf2);
}

void
test_cut (void)
{
    char str1[] = "\x01\x01\x01x234x\x01\x01y\x01\x01xy123\x01\x01";
    char *str2 = (char *)NULL;
    char *str3 = (char *)NULL;

    cbuffer_t *buf1 = (cbuffer_t *)NULL;
    cbuffer_t *buf2 = (cbuffer_t *)NULL;

    buf1 = cbuf_new();

    cbuf_import (buf1, str1, strlen (str1));
    buf2 = cbuf_cut (buf1, 2, 14);

    printf ("test_cut(): str1 = %s\n", str1);

    str2 = (char *)xmalloc (buf1->sz + 1);
    memset (str2, (int)NULL, buf1->sz + 1);
    memcpy (str2, buf1->data, buf1->sz);

    str3 = (char *)xmalloc (buf2->sz + 1);
    memset (str3, (int)NULL, buf2->sz + 1);
    memcpy (str3, buf2->data, buf2->sz);

    printf ("test_cut(): str2 = %s\ntest_cut(): str3 = %s\n", str2, str3);

    xfree (str2);
    xfree (str3);

    cbuf_delete (buf1);
    cbuf_delete (buf2);
}

void
test_search (void)
{
    char str1[] = "\x01\x01x234x\x01\x01y\x01\x01xy123\x01\x01";
    lstdl_t *lst = (lstdl_t *)NULL;

    cbuffer_t *buf1 = (cbuffer_t *)NULL;
    buf1 = cbuf_new();
    cbuf_import (buf1, str1, strlen (str1));

    lst = cbuf_search (buf1, "\x01", 1);

    printf ("test_search(): len = %d\n", lstdl_length (lst));

    lstdl_delete_nocb (lst);
    cbuf_delete (buf1);
}

void
test_tail_head (void)
{
    char str1[] = "123456";
    char *str2 = (char *)NULL;
    char *str3 = (char *)NULL;

    cbuffer_t *buf1;
    cbuffer_t *buf2;
    cbuffer_t *buf3;
    buf1 = (cbuffer_t *)NULL;
    buf2 = (cbuffer_t *)NULL;
    buf3 = (cbuffer_t *)NULL;

    printf ("test_tail_head(): str1 = %s\n", (char *)str1);

    buf1 = cbuf_new();
    cbuf_import (buf1, str1, strlen (str1));

    buf2 = cbuf_head (buf1, 3);
    str2 = (char *)malloc (buf2->sz + 1);
    if (str2 != (char *)NULL) {
        memset (str2, (int)NULL, buf2->sz + 1);
        memcpy (str2, buf2->data, buf2->sz);
        printf ("test_tail_head(): str2 = %s\n", str2);
        free (str2);
    }

    buf3 = cbuf_tail (buf1, 3);
    str3 = (char *)malloc (buf3->sz + 1);
    if (str3 != (char *)NULL) {
        memset (str3, (int)NULL, buf3->sz + 1);
        memcpy (str3, buf3->data, buf3->sz);
        printf ("test_tail_head(): str3 = %s\n", str3);
        free (str3);
    }

    cbuf_delete (buf2);
    cbuf_delete (buf3);

    buf2 = cbuf_head_cut (buf1, 3);
    buf3 = cbuf_tail_cut (buf2, 3);

    cbuf_delete (buf2);
    cbuf_delete (buf3);

    cbuf_delete (buf1);
}

/** }@ */
/* caf_buffer.c ends here */
