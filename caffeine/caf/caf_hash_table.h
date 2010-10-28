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
#include <caf/caf_data_cdeque.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Hash structure size @see caf_hash_t */
#define CAF_HASH_SZ                 (sizeof (caf_hash_t))
/** Hash table structure size @see caf_hash_table_t */
#define CAF_HASH_TABLE_SZ           (sizeof (caf_hash_table_t))

/**
 * @brief		Double Hash Structure Type.
 *
 * The double hash structure type.
 *
 * @see caf_hash_s
 */
typedef struct caf_hash_s caf_hash_t;

/**
 * @brief		Double Hash Structure.
 *
 * The double hash structure.
 *
 * @see caf_hash_t
 */
struct caf_hash_s {
	/** First Hash of the Double Hash Structure */
	u_int32_t hash1;
	/** Second Hash of the Double Hash Structure */
	u_int32_t hash2;
	/** Key Size */
	size_t key_sz;
	/** Key Pointer */
	void *key;
	/** Data Pointer */
	void *data;
};

/**
 * @brief Hash Table Structure Type.
 *
 * Hash Table structure type.
 *
 * @see caf_hash_table_s
 */
typedef struct caf_hash_table_s caf_hash_table_t;

/**
 * @brief Hash Table Strcture
 *
 * Hash Table Structure.
 *
 * @see caf_hash_table_t
 */
struct caf_hash_table_s {
    /** Hash Table Identifier */
	int id;
	/** First Hash Callback Function */
	CAF_HASH_STR_FUNCTION(f1);
	/** Second Hash Callback Function */
	CAF_HASH_STR_FUNCTION(f2);
	/** Hash List */
	cdeque_t *hashes;
};

/**
 * @brief Creates a new hash node.
 *
 * Creates a new hash container using the <b>caf_hash_t</b> structure
 * and calculates the double hashing using the callback functions
 * <b>f1</b> and <b>f2</b>. The <b>key</b> is any pointer to string
 * data and the <b>ksz</b> is the key size. <b>data</b> is the data
 * pointed to reach with <b>key</b>.
 *
 * @param key[in]						key pointer
 * @param ksz[in]						key size
 * @param data[in]						data pointer
 * @param CAF_HASH_STR_FUNCTION[in]		hash callback 1
 * @param CAF_HASH_STR_FUNCTION[in]		hash callback 2
 *
 * @return caf_hash_t					a new allocated/calculated hash
 */
caf_hash_t *caf_hash_new (const void *key, const size_t ksz, const void *data,
                          CAF_HASH_STR_FUNCTION(f1),
                          CAF_HASH_STR_FUNCTION(f2));

/**
 * @brief Creates an empy hash node.
 *
 * Creates a new hash container using the <b>caf_hash_t</b> structure
 * and calculates the double hashing using the callback functions
 * <b>f1</b> and <b>f2</b>. The <b>key</b> is any pointer to string
 * data and the <b>ksz</b> is the key size. There is no data pointed
 * by the double hash.
 *
 * @param key[in]						key pointer
 * @param ksz[in]						key size
 * @param CAF_HASH_STR_FUNCTION[in]		hash callback 1
 * @param CAF_HASH_STR_FUNCTION[in]		hash callback 2
 *
 * @return caf_hash_t					a new allocated hash
 */
caf_hash_t *caf_hash_new_nodata (const void *key, const size_t ksz,
                                 CAF_HASH_STR_FUNCTION(f1),
                                 CAF_HASH_STR_FUNCTION(f2));

/**
 * @brief Deallocates a Hash Pointer
 *
 * Deallocates the memory given by the hash node <b>caf_hash_t</b>
 * argument <b>hash</b>.
 *
 * @param hash[in]			hash node to deallocate.
 *
 * @return int				CAF_OK on success, CAF_ERROR on failure
 */
int caf_hash_delete (caf_hash_t *hash);

/**
 * @brief Creates a new empty hash table.
 *
 * Creates a new empty hash table. The hash table implementation
 * it's <b>caf_cdeque_t</b> based. This means that search algorithms
 * are linear, and using the <b>Circular Linked List</b> sequence.
 * Future implementations will include fater search methods and
 * faster data type structures.
 *
 * @param id[in]						Hash Table Identifier
 * @param CAF_HASH_STR_FUNCTION[in]		Hash Callback 1
 * @param CAF_HASH_STR_FUNCTION[in]		Hash Callback 2
 *
 * @return caf_hash_table_t				a new allocated table
 *
 * @see cdeque_t
 * @see caf_hash_table_t
 * @see caf_hash_t
 */
caf_hash_table_t *caf_hash_table_new (const int id,
                                      CAF_HASH_STR_FUNCTION(f1),
                                      CAF_HASH_STR_FUNCTION(f2));

/**
 * @brief Deallocates a Hash Table
 *
 * Deallocates the pointer of the given Hash Table <b>table</b>,
 * restoring the allocated memory through <b>free(2)</b>.
 *
 * @param table[in]		table to deallocate
 *
 * @return int			CAF_OK on success, CAF_ERROR on failure
 */
int caf_hash_table_delete (caf_hash_table_t *table);

/**
 * @brief Adds a hash to the given table
 *
 * Adds a new hash to the given table <b>table</b>. You don't
 * need to pass the entire <b>caf_hash_t</b> structure, instead,
 * you must pass the data confirming the hash, the <b>key</b>
 * and <b>data</b> pair. Also, the key size <b>ksz</b> is
 * needed.
 *
 * @param table[in]		table to add the hash
 * @param key[in]		key pointer
 * @param ksz[in]		key size pointer
 * @param data[in]		data pointer
 *
 * @return int			CAF_OK on success, CAF_ERROR on failure
 */
int caf_hash_table_add (caf_hash_table_t *table, const void *key,
                        const size_t ksz, const void *data);

/**
 * @brief Removes an element from the given Hash Table
 *
 * Removes the element identified by the hash key <b>key</b> of size
 * <b>ksz</b> from the given hash table <b>table</b>. This deallocates
 * the <b>caf_hash_t</b> pointer and removes the list node, but does
 * not deallocates the data and key pointers.
 *
 * @param table[in]		table from where to remove the item
 * @param key[in]		item key <b>data</b> string
 * @param ksz[in]		key size
 *
 * @return int			CAF_OK on success, CAF_ERROR on failure
 */
int caf_hash_table_remove (caf_hash_table_t *table, const void *key,
                           const size_t ksz);

/**
 * @brief Obtains a the data from the given hash table
 *
 * Obtains the data pointer from the given hash table using
 * the given key <b>key</b> with the key size of <b>ksz</b>
 * to locate the the key in the given hash table <b>table</b>
 * using the hash table callback to calculate the hash
 * values of the double hashing <b>caf_hash_t</b> member.
 *
 * @param table[in]		hash table where to search the node
 * @param key[in]		key to search
 * @param ksz[in]		key size of the given key
 *
 * @return void *		data pointer on success, NULL on failure
 */
void *caf_hash_table_get (caf_hash_table_t *table, const void *key,
                          const size_t ksz);

/**
 * @brief Replaces the data pointer for the given key
 *
 * Replaces the data on the found node by the key <b>key</b> and
 * the key size <b>ksz</b>. If the key isn't found, the interface
 * fails.
 *
 * @param table[in]		hash table
 * @param key[in]		key to search
 * @param ksz[in]		key size
 * @param data[in]		data to replace for
 *
 * @return int			CAF_OK on success, CAF_ERROR on failure
 */
int caf_hash_table_set (caf_hash_table_t *table, const void *key,
                        const size_t ksz, void *data);

/**
 * @brief Dumps the hash table contents to the given output
 *
 * Dumps the given hash table <b>table</b> contents to the
 * given FILE output. Do not dumps the hash table data, instead
 * dumps the pointer information on the table in developer
 * friendly format.
 *
 * @param out[in]		file to write
 * @param table[in]		table to dump
 */
void caf_hash_table_dump (FILE *out, caf_hash_table_t *table);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_HASH_TABLE_H */
/* caf_hash_table.h ends here */


