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

#include <caf/caf_hash_str.h>
#include <caf/caf_hash_table.h>


#define TABLE_ID            1000

int
main ()
{
    caf_hash_table_t *table = (caf_hash_table_t *)NULL;
    table = caf_hash_table_new (TABLE_ID, caf_shash_dek, caf_shash_fnv);
    if (table != (caf_hash_table_t *)NULL) {
        caf_hash_table_add (table, "hello", strlen("hello") + 1, "hello 1");
        caf_hash_table_add (table, "bye", strlen("bye") + 1, "bye 1");
        caf_hash_table_add (table, "hola", strlen("hola") + 1, "hola 1");
        caf_hash_table_add (table, "chao", strlen("chao") + 1, "chao 1");
        caf_hash_table_add (table, "hola", strlen("hola") + 1, "hola 2");
        caf_hash_table_add (table, "hola", strlen("hola") + 1, "hola 3");
        caf_hash_table_add (table, "hello", strlen("hello") + 1, "hello 2");
        caf_hash_table_add (table, "chao", strlen("chao") + 1, "chao 2");
        caf_hash_table_set (table, "bye", strlen("bye") + 1, "bye 2");
        caf_hash_table_dump (stdout, table);
        printf ("remove: %d\n", caf_hash_table_remove (table, "hello",
                                                       strlen("hello") + 1));
        caf_hash_table_dump (stdout, table);
        lstdlc_dump_ptr (stdout, table->hashes);
        caf_hash_table_delete (table);
    }
    return 0;
}

/* caf_hash_tabel.c ends here */
