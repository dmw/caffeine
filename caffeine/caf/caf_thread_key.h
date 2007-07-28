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

    $Id$
 */
#ifndef CAF_THREAD_KEY_H
#define CAF_THREAD_KEY_H 1

#include <pthread.h>
#include <caf/caf_data_lstdl.h>

/**
 * @defgroup      caf_thread_key    Thread Key
 * @ingroup       caf_thread
 * @addtogroup    caf_thread_key
 * @{
 *
 * @brief     Thread Key.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Manage Thread Keys (TSD). Allow the program to create key pools
 * instantly released on pool destruction, otherwise, if the key
 * release callback is set, the normal pthread behavior does the
 * TSD release.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

	/** Defines the key data destruction prototype */
#define PTH_KEY_DESTROY_RTN(rtn)                void (*rtn)(void *)
	/** Defines the pth_key_t structure size */
#define PTH_KEY_T_SZ                            sizeof(pth_key_t)
	/** Defines the pth_kpool_t structure size */
#define PTH_KPOOL_T_SZ                          sizeof(pth_kpool_t)


	/**
	 *
	 * @brief    Caffeine Thread Key Type.
	 * The type of a thread key.
	 * @see      pth_key
	 */
	typedef struct pth_key_s pth_key_t;
	/**
	 *
	 * @brief    Caffeine Thread Key Structure.
	 * A Thread Key Structure stores the thread specific data for a key,
	 * with the enought information to manage the thread specific data,
	 * allowing the program to know relevant data about the thread specific
	 * data.
	 */
	struct pth_key_s {
		/** Key Identifier */
		int id;
		/** Key Pointer */
		void *ptr;
		/** Pointer Size */
		size_t sz;
		/** Thread Key */
		pthread_key_t key;
		/** Pointer Destruction Routine */
		PTH_KEY_DESTROY_RTN(rtn);
	};

	/**
	 *
	 * @brief    Caffeine Thread Key Pool Type.
	 * The type of a thread key pool.
	 * @see      pth_kpool
	 */
	typedef struct pth_kpool_s pth_kpool_t;
	/**
	 *
	 * @brief    Caffeine Thread Key Pool Structure.
	 * The key pool is an pool identifier asociated with a thread, that holds
	 * a list of keys -- thread specific data -- that allows a thread to destroy
	 * the data asociated with a cleanup routine.
	 */
	struct pth_kpool_s {
		/** The Pool Identifier */
		int id;
		/** The Asociated Thread */
		pthread_t *thr;
		/** The Key List */
		lstdl_t *keys;
	};


	/**
	 *
	 * @brief    Creates a new Caffeine Thread Key Instance.
	 *
	 * Allocates memory for a pth_key_t structure, setting the identifier of
	 * the key and the key data destruction routine.
	 *
	 * @param[in]    id              key identifier.
	 * @param[in]    rtn             key data destruction routine.
	 * @return       pth_key_t *     the allocated pth_key_t structure.
	 *
	 * @see      pth_key_t
	 */
	pth_key_t *pth_key_new (int id, PTH_KEY_DESTROY_RTN(rtn));

	/**
	 *
	 * @brief    Deletes a the given Caffeine Thread Key Instance.
	 *
	 * Deallocates memory for a pth_key_t structure, using the key destruction
	 * routine, is assumed that the key data destruction routine is called by
	 * key destruction routine.
	 *
	 * @param[in]    k               Caffeine Thread Key.
	 * @return       int             zero on success.
	 *
	 * @see      pth_key_t
	 */
	int pth_key_delete (pth_key_t *k);

	/**
	 *
	 * @brief    Sets the key specific data.
	 *
	 * Allocates the key specific data, but does not sets the data contents,
	 * only allocates memory for the key.
	 *
	 * @param[in]    key             Caffeine Thread Key.
	 * @param[in]    sz              data size.
	 * @return       int             zero on success.
	 *
	 * @see      pth_key_t
	 */
	int pth_key_set (pth_key_t *key, size_t sz);

	/**
	 *
	 * @brief    Gets the key specific data.
	 *
	 * Gets the pointer to the key specific data.
	 *
	 * @param[in]    key             Caffeine Thread Key.
	 * @return       void *          Key specific data pointer, NULL on error.
	 *
	 * @see      pth_key_t
	 */
	void *pth_key_get (pth_key_t *key);

	/**
	 *
	 * @brief    Creates a new Thread Key Pool instance.
	 *
	 * Allocates memory for a Key Pool instance, setting the identifier of the
	 * pool and the thread that links to the key pool.
	 *
	 * @param[in]    id              Key Pool Identifier.
	 * @param[in]    thr             Thread linked with the pool.
	 * @return       pth_kpool_t *   Pointer of a new pth_kpool_t structure.
	 *
	 * @see      pth_kpool_t
	 * @see      pth_key_t
	 */
	pth_kpool_t *pth_kpool_new (int id, pthread_t *thr);

	/**
	 *
	 * @brief    Delete the given Key Pool.
	 *
	 * Deallocates the given key pool, destroying the contents.
	 *
	 * @param[in]    kpool           Thread Key Pool.
	 *
	 * @see      pth_kpool_t
	 * @see      pth_key_t
	 */
	void pth_kpool_delete (pth_kpool_t *kpool);

	/**
	 *
	 * @brief    Adds a new key to the given pool.
	 *
	 * Adds a new key to the given pool.
	 *
	 * @param[in]    pool           Thread Key Pool.
	 * @param[in]    key            Thread Key.
	 * @return       void *         Pointer of a new pth_kpool_t structure.
	 *
	 * @see      pth_kpool_t
	 * @see      pth_key_t
	 */
	pth_key_t *pth_kpool_get_key (pth_kpool_t *pool, int id);

	/**
	 *
	 * @brief    Search and returns a key in the pool using the key identifier.
	 *
	 * Search the key list and returns the key asociated with the given identifier.
	 *
	 * @param[in]    pool           Thread Key Pool.
	 * @param[in]    id             Key identifier.
	 * @return       pth_key_t *    the found key, NULL on failure.
	 *
	 * @see      pth_kpool_t
	 * @see      pth_key_t
	 */
	void *pth_kpool_get (pth_kpool_t *pool, int id);

	/**
	 *
	 * @brief    Search and returns key specific data in the pool.
	 *
	 * Search the key list and returns the key specific data asociated with
	 * the given identifier.
	 *
	 * @param[in]    pool           Thread Key Pool.
	 * @param[in]    id             Key identifier.
	 * @return       void *         the found data pointer, NULL on failure.
	 *
	 * @see      pth_kpool_t
	 * @see      pth_key_t
	 */
	void *pth_kpool_add (pth_kpool_t *pool, pth_key_t *key);

	/**
	 *
	 * @brief    Removes a key using the the key identifier.
	 *
	 * Search for a key using the key identifier in the pool and removes the
	 * key from the pool, destroying the found key.
	 *
	 * @param[in]    pool           Thread Key Pool.
	 * @param[in]    id             Key identifier.
	 *
	 * @see      pth_kpool_t
	 * @see      pth_key_t
	 */
	void pth_kpool_remove (pth_kpool_t *pool, pth_key_t *key);

	/**
	 *
	 * @brief    Removes a key using the the key pointer itself.
	 *
	 * Search for a key using the key pointer itself in the pool and removes the
	 * key from the pool, destroying the found key.
	 *
	 * @param[in]    pool           Thread Key Pool.
	 * @param[in]    id             Key identifier.
	 *
	 * @see      pth_kpool_t
	 * @see      pth_key_t
	 */
	void pth_kpool_remove_by_id (pth_kpool_t *pool, int id);

	/**
	 *
	 * @brief    Callback used to destroy found keys in the pool.
	 *
	 * Callback used to destroy keys found in the pool.
	 *
	 * @param[in]    k              Thread Key.
	 * @param[in]    id             one on success, zero on failure.
	 *
	 * @see      pth_kpool_t
	 * @see      pth_key_t
	 */
	int pth_kpool_remove_callback (void *k);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_THREAD_KEY_H */
/* caf_thread_key.h ends here */
