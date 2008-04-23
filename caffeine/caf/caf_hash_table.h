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

  $Id$
*/
#ifndef CAF_HASH_TABLE_H
#define CAF_HASH_TABLE_H 1
/**
 * @defgroup      caf_hash_table    Hash Table Functions
 * @ingroup       caf_data_struct
 * @addtogroup    caf_hash_table
 * @{
 *
 * @brief     String Hash Functions.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@coder.cl>
 *
 * Hash Table Functions
 *
 */

#include <sys/types.h>

#include <caf/caf_data_lstdlc.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Hash structure size @see caf_hash_t */
#define CAF_HASH_SZ                 (sizeof (caf_hash_t))
/** Hash table structure size @see caf_hash_table_t */
#define CAF_HASH_TABLE_SZ           (sizeof (caf_hash_table_t))

typedef struct caf_hash_s caf_hash_t;
struct caf_hash_s {
	u_int32_t hash1;
	u_int32_t hash2;
	size_t key_sz;
	void *key;
	void *data;
};

typedef struct caf_hash_table_s caf_hash_table_t;
struct caf_hash_table_s {
	int id;
	CAF_HASH_STR_FUNCTION(f1);
	CAF_HASH_STR_FUNCTION(f2);
	lstdlc_t *hashes;
};

caf_hash_t *caf_hash_new (const void *key, const size_t ksz, const void *data,
                          CAF_HASH_STR_FUNCTION(f1),
                          CAF_HASH_STR_FUNCTION(f2));
caf_hash_t *caf_hash_new_nodata (const void *key, const size_t ksz,
                                 CAF_HASH_STR_FUNCTION(f1),
                                 CAF_HASH_STR_FUNCTION(f2));
int caf_hash_delete (caf_hash_t *hash);
caf_hash_table_t *caf_hash_table_new (const int id,
                                      CAF_HASH_STR_FUNCTION(f1),
                                      CAF_HASH_STR_FUNCTION(f2));
int caf_hash_table_delete (caf_hash_table_t *table);
int caf_hash_table_add (caf_hash_table_t *table, const void *key,
                        const size_t ksz, const void *data);
int caf_hash_table_remove (caf_hash_table_t *table, const void *key,
                           const size_t ksz);
void *caf_hash_table_get (caf_hash_table_t *table, const void *key,
                          const size_t ksz);
int caf_hash_table_set (caf_hash_table_t *table, const void *key,
                        const size_t ksz, void *data);
void caf_hash_table_dump (FILE *out, caf_hash_table_t *table);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_HASH_TABLE_H */
/* caf_hash_table.h ends here */

