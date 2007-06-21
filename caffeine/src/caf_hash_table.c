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

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_hash_str.h"
#include "caf/caf_hash_table.h"


static int caf_hash_search_callback (void *lptr, void *data);
static int caf_hash_delete_callback (void *data);
static int caf_hash_dump (FILE *out, void *data);


caf_hash_t *
caf_hash_new (const void *key, const size_t ksz, const void *data,
              CAF_HASH_STR_FUNCTION(f1),
              CAF_HASH_STR_FUNCTION(f2))
{
    caf_hash_t *r = (caf_hash_t *)NULL;
    if (key != (const void *)NULL && ksz > 0 && data != (const void *)NULL
        && f1 != NULL && f2 != NULL) {
        r = (caf_hash_t *)xmalloc (CAF_HASH_SZ);
        if (r != (caf_hash_t *)NULL) {
            r->hash1 = f1 ((const char *)key, ksz);
            r->hash2 = f2 ((const char *)key, ksz);
            r->key_sz = ksz;
            r->key = (void *)key;
            r->data = (void *)data;
        }
    }
    return r;
}


caf_hash_t *caf_hash_new_nodata (const void *key, const size_t ksz,
                                        CAF_HASH_STR_FUNCTION(f1),
                                        CAF_HASH_STR_FUNCTION(f2))
{
    caf_hash_t *r = (caf_hash_t *)NULL;
    if (key != (const void *)NULL && ksz > 0 && f1 != NULL && f2 != NULL) {
        r = (caf_hash_t *)xmalloc (CAF_HASH_SZ);
        if (r != (caf_hash_t *)NULL) {
            r->hash1 = f1 ((const char *)key, ksz);
            r->hash2 = f2 ((const char *)key, ksz);
            r->key_sz = ksz;
            r->key = (void *)key;
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
caf_hash_table_new (const int id, CAF_HASH_STR_FUNCTION(f1),
                    CAF_HASH_STR_FUNCTION(f2))
{
    caf_hash_table_t *r = (caf_hash_table_t *)NULL;
    if (id > 0 && f1 != NULL) {
        r = (caf_hash_table_t *)xmalloc (CAF_HASH_TABLE_SZ);
        if (r != (caf_hash_table_t *)NULL) {
            r->id = id;
            r->f1 = f1;
            r->f2 = f2;
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
    caf_hash_t *hash_new;
    if (table != (caf_hash_table_t *)NULL && key != (const void *)NULL &&
        ksz > 0 && data != (const void *)NULL) {
        hash_new = caf_hash_new (key, ksz, data, table->f1, table->f2);
        if ((hash = (caf_hash_t *)lstdlc_search (table->hashes,
                                                 (void *)hash_new,
                                                 caf_hash_search_callback))
            == (caf_hash_t *)NULL) {
            hash = caf_hash_new (key, ksz, data, table->f1, table->f2);
            if (hash != (caf_hash_t *)NULL) {
                caf_hash_delete (hash_new);
                return ((lstdlc_push (table->hashes, (void *)hash))
                        != (lstdlc_t *)NULL) ? CAF_OK : CAF_ERROR;
            }
        } else {
            hash->key = (void *)key;
            hash->data = (void *)data;
            hash->hash1 = hash_new->hash1;
            hash->hash2 = hash_new->hash2;
            hash->key_sz = ksz;
            caf_hash_delete (hash_new);
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


int
caf_hash_table_remove (caf_hash_table_t *table, const void *key,
                       const size_t ksz)
{
    caf_hash_t *hash;
    caf_hash_t *hash_new;
    if (table != (caf_hash_table_t *)NULL && key != (const void *)NULL &&
        ksz > 0) {
        hash_new = caf_hash_new_nodata (key, ksz, table->f1, table->f2);
        if ((hash = (caf_hash_t *)lstdlc_search (table->hashes,
                                                 (void *)hash_new,
                                                 caf_hash_search_callback))
            != (caf_hash_t *)NULL) {
            caf_hash_delete (hash_new);
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
    caf_hash_t *hash_new;
    void *r = (void *)NULL;
    if (table != (caf_hash_table_t *)NULL && key != (const void *)NULL &&
        ksz > 0) {
        hash_new = caf_hash_new_nodata (key, ksz, table->f1, table->f2);
        if ((hash = (caf_hash_t *)lstdlc_search (table->hashes,
                                                 (void *)hash_new,
                                                 caf_hash_search_callback))
            != (caf_hash_t *)NULL) {
            caf_hash_delete (hash_new);
            return hash->data;
        }
    }
    return r;
}


int
caf_hash_table_set (caf_hash_table_t *table, const void *key,
                    const size_t ksz, void *data)
{
    caf_hash_t *hash;
    caf_hash_t *hash_new;
    if (table != (caf_hash_table_t *)NULL && key != (const void *)NULL &&
        ksz > 0) {
        hash_new = caf_hash_new_nodata (key, ksz, table->f1, table->f2);
        if ((hash = (caf_hash_t *)lstdlc_search (table->hashes,
                                                 (void *)hash_new,
                                                 caf_hash_search_callback))
            != (caf_hash_t *)NULL) {
            hash->data = (void *)data;
            hash->key = (void *)key;
            hash->key_sz = ksz;
            hash->hash1 = hash_new->hash1;
            hash->hash2 = hash_new->hash2;
            caf_hash_delete (hash_new);
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


void
caf_hash_table_dump (FILE *out, caf_hash_table_t *table)
{
    if (table != (caf_hash_table_t *)NULL) {
        fprintf (out, "[%p] Hash Table\n", (void *)table);
        lstdlc_dump (out, table->hashes, caf_hash_dump);
    }
}


static int
caf_hash_search_callback (void *lptr, void *data)
{
    caf_hash_t *hash1 = (caf_hash_t *)lptr;
    caf_hash_t *hash2 = (caf_hash_t *)data;
    return (hash1->hash1 == hash2->hash1
            && hash1->hash2 == hash2->hash2)
            ? CAF_OK : CAF_ERROR;
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

static int
caf_hash_dump (FILE *out, void *data)
{
    caf_hash_t *hash;
    const char *msg = "[%p] hash1: %15.15u; hash2: %15.15u; key: %30.30s\n"
                      "     data: %s\n\n";
    if (data != (void *)NULL) {
        hash = (caf_hash_t *)data;
        fprintf (out, msg, hash, hash->hash1, hash->hash2, (char *)hash->key,
                 (char *)data);
        return CAF_OK;
    }
    return CAF_ERROR;
}

/* caf_hash_table.c ends here */
