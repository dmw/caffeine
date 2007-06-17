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
#ifndef lint
static char Id[] = "$Id$";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <sys/types.h>

#include "caf/caf_data_mem.h"
#include "caf/caf_hash_str.h"
#include "caf/caf_hash_table.h"


static int caf_hash_search_callback (void *lptr, void *data);
static int caf_hash_delete_callback (void *data);


caf_hash_t *
caf_hash_new (const void *key, const size_t ksz, const void *data,
              caf_hash_function_t *function)
{
    caf_hash_t *r = (caf_hash_t *)NULL;
    if (key != (const void *)NULL && ksz > 0 && data != (const void *data)
        && function != (caf_hash_function_t *)NULL) {
        r = (caf_hash_t *)xmalloc (CAF_HASH_SZ);
        if (r != (caf_hash_t *)NULL) {
            r->hash = function ((const char *)key, ksz);
            r->key_sz = ksz;
            r->key = key;
            r->data = data;
        }
    }
    return r;
}


int
caf_hash_delete (caf_hash_t *hash)
{
    if (hash != (caf_hash_t *)NULL) {
        xfree (hash);
        return CAF_OK;
    }
    return CAF_ERROR;
}


caf_hash_table_t *
caf_hash_table_new (const int id, caf_hash_function_t *function)
{
    caf_hash_table_t *r = (caf_hash_table_t *)NULL;
    if (id > 0 && function != (caf_hash_function_t *)NULL) {
        r = (caf_hash_table_t *)xmalloc (CAF_HASH_TABLE_SZ);
        if (r != (caf_hash_table_t *)NULL) {
            r->id = id;
            r->function = function;
            r->hashes = lstdlc_create ();
            if (r->hashes == (lstdlc_t *)NULL) {
                xfree (r);
                r = (caf_hash_table_t *)NULL;
            }
        }
    }
    return r;
}


int
caf_hash_table_delete (caf_hash_table_t *table)
{
    if (table != (caf_hash_table_t *)NULL) {
        if ((lstdlc_delete_nocb (table->hashes)) == CAF_OK) {
            xfree (table);
            return CAF_OK;
        }
    }
    return CAF_OK;
}


int
caf_hash_table_add (caf_hash_table_t *table, const void *key,
                    const size_t ksz, const void *data)
{
    caf_hash_t *hash;
    u_int32_t calc;
    if (table != (caf_hash_table_t *)NULL && key != (const void *)NULL &&
        ksz > 0 && data != (const void *)NULL) {
        calc = table->function ((const char *)key, ksz);
        if ((hash = (caf_hash_t *)lstdlc_search (table->hashes, (void *)&calc,
                                                 caf_hash_search_callback))
            == (caf_hash_t *)NULL) {
            hash = caf_hash_new (key, ksz, data);
            if (hash != (caf_hash_t *)NULL) {
                return lstdlc_push (table, (void *)hash);
            }
        } else {
            hash->key = key;
            hash->data = data;
            hash->hash = calc;
            hash->key_sz = ksz;
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


int
caf_hash_table_remove (caf_hash_table_t *table, const void *key,
                       const size_t *ksz)
{
    caf_hash_t *hash;
    u_int32_t calc;
    if (table != (caf_hash_table_t *)NULL && key != (const void *)NULL &&
        ksz > 0 && data != (const void *)NULL) {
        calc = table->function ((const char *)key, ksz);
        if ((hash = (caf_hash_t *)lstdlc_search (table->hashes, (void *)&calc,
                                                 caf_hash_search_callback))
            != (caf_hash_t *)NULL) {
            return lstdlc_node_delete_by_data (table->hashes, (void *)hash,
                                               caf_hash_delete_callback);
        }
    }
    return CAF_ERROR;
}


void *
caf_hash_table_get (caf_hash_table_t *table, const void *key,
                    const size_t ksz)
{
    caf_hash_t *hash;
    void *r = (void *)NULL;
    u_int32_t calc;
    if (table != (caf_hash_table_t *)NULL && key != (const void *)NULL &&
        ksz > 0) {
        calc = table->function ((const char *)key, ksz);
        if ((hash = (caf_hash_t *)lstdlc_search (table->hashes, (void *)&calc,
                                                 caf_hash_search_callback))
            != (caf_hash_t *)NULL) {
            return hash->data;
        }
    }
    return r;
}


int
caf_hash_table_set (caf_hash_table_t *table, const void *key,
                    const size_t *ksz, void *data)
{
    caf_hash_t *hash;
    void *r = (void *)NULL;
    u_int32_t calc;
    if (table != (caf_hash_table_t *)NULL && key != (const void *)NULL &&
        ksz > 0) {
        if ((hash = (caf_hash_t *)lstdlc_search (table->hashes, (void *)&calc,
                                                 caf_hash_search_callback))
            != (caf_hash_t *)NULL) {
            hash->data = data;
            hash->key_sz = ksz;
            hash->key = key;
            hash->hash = hash;
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


static int
caf_hash_search_callback (void *lptr, void *data)
{
    caf_hash_t *hash = (caf_hash_t *)lptr;
    return hash->hash == (u_int32_t)*data ? CAF_OK : CAF_ERROR;
}


static int
caf_hash_delete_callback (void *data)
{
    if (data != (void *)NULL) {
        xfree (data);
        return CAF_OK;
    }
    return CAF_ERROR;
}

/* caf_hash_table.c ends here */
