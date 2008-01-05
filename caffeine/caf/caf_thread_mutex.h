/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "caf" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
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
#ifndef CAF_THREAD_MUTEX_H
#define CAF_THREAD_MUTEX_H 1

#include <pthread.h>

/**
 * @defgroup      caf_thread_mutex    Thread Mutexes
 * @ingroup       caf_thread
 * @addtogroup    caf_thread_mutex
 * @{
 *
 * @brief     Thread Mutexes.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * This module manage specific mutexes and his attributes. The idea is to
 * maintain the mutex and his attributes. Also a bitfield to manage
 * get information about the attributes that has been set for the mutex.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Defines the pth_mutex_t structure size */
#define CAF_PTH_MTX_SZ            sizeof(pth_mutex_t)

/**
 *
 * @brief    Defines the pth_mutexattr_sets mutex properties
 * Defines the types that can be used with pth_mutex_t structures.
 */
typedef enum {
	/** pthread_attr_(set|get)detachstate */
	PTHDR_MUTEX_PRIOCEIL = 0000001,
	/** pthread_attr_(set|get)detachstate */
	PTHDR_MUTEX_PROTO = 0000002,
	/** pthread_attr_(set|get)schedparam */
	PTHDR_MUTEX_TYPE = 0000004,
	/** locked */
	PTHDR_MUTEX_LOCK = 0000010
} pth_mutexattr_types_t;

/**
 *
 * @brief    Caffeine Thread Mutex Type.
 * The type of a thread mutex.
 * @see      pth_mutex
 */
typedef struct pth_mutex_s pth_mutex_t;
/**
 *
 * @brief    Caffeine Thread Mutex Structure.
 * This structure holds the mutex attributes flags that contains the mutex
 * attributes sets for the mutex. A mutex attribute structure and the
 * mutex structure.
 */
struct pth_mutex_s {
	/** Mutex Attributes Flags */
	int at;
	/** Mutex Attributes */
	pthread_mutexattr_t attr;
	/** The Thread Mutex */
	pthread_mutex_t mutex;
};

/**
 *
 * @brief    Creates a new Caffeine Thread Mutex Instance.
 *
 * Allocates memory for a pth_mutex_t structure.
 *
 * @return       pth_mutex_t *     the allocated pth_mutex_t structure.
 *
 * @see      pth_mutex_t
 */
pth_mutex_t *pth_mtx_new ();

/**
 *
 * @brief    Deletes a new Caffeine Thread Mutex Instance.
 *
 * Deallocates memory for a pth_mutex_t structure.
 *
 * @param[in]    mtx               pointer to the allocated mutex.
 * @return       pth_mutex_t *     the allocated pth_mutex_t structure.
 *
 * @see      pth_mutex_t
 */
void pth_mtx_delete (pth_mutex_t *mtx);

/**
 *
 * @brief    Inits the Caffeine Mutex Attributes on the given Mutex Instance
 *
 * Initis the mutex attributes for the given mutex instance.
 *
 * @param[in]    mtx             mutex instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_mutex_t
 */
int pth_mtxattr_init (pth_mutex_t *mtx);

/**
 *
 * @brief    Destroys the Caffeine Mutex Attributes on the given Mutex Instance
 *
 * Destroys the mutex attributes for the given mutex instance.
 *
 * @param[in]    mtx             mutex instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_mutex_t
 */
int pth_mtxattr_destroy (pth_mutex_t *mtx);

/**
 *
 * @brief    Initializes the mutex in the given Caffeine Mutex Instance
 *
 * Initializes the mutex in the given Caffeine Mutex Instance.
 *
 * @param[in]    mtx             mutex instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_mutex_t
 */
int pth_mtx_init (pth_mutex_t *mtx);

/**
 *
 * @brief    Destroys the mutex in the given Caffeine Mutex Instance
 *
 * Initializes the mutex in the given Caffeine Mutex Instance.
 *
 * @param[in]    mtx             mutex instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_mutex_t
 */
int pth_mtx_destroy (pth_mutex_t *mtx);

/**
 *
 * @brief    Sets an attribute for the given Caffeine Mutex Instance
 *
 * Sets an attribute for the given Caffeine Mutex Instance, with a type
 * of attribute and atrribute value.
 *
 * @param[in]    mtx             mutex instance to init attributes.
 * @param[in]    t               attribute type.
 * @param[in]    data            data to use with the mutex.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_mutex_t
 */
int pth_mtxattr_set (pth_mutex_t *mtx, pth_mutexattr_types_t t, int data);

/**
 *
 * @brief    Gets an attribute for the given Caffeine Mutex Instance
 *
 * Gets an attribute for the given Caffeine Mutex Instance, with a type
 * of attribute and atrribute value.
 *
 * @param[in]    mtx             mutex instance to init attributes.
 * @param[in]    t               attribute type.
 * @param[out]   data            pointer to store attribute data.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_mutex_t
 */
int pth_mtxattr_get (pth_mutex_t *mtx, pth_mutexattr_types_t t, int *data);

/**
 *
 * @brief    Try to lock the Mutex for the given Caffeine Mutex Instance
 *
 * Try to lock the Mutex for the given Caffeine Mutex Instance
 *
 * @param[in]    mtx             mutex instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_mutex_t
 */
int pth_mtx_trylock (pth_mutex_t *mtx);

/**
 *
 * @brief    Locks the Mutex for the given Caffeine Mutex Instance
 *
 * Locks the Mutex for the given Caffeine Mutex Instance.
 *
 * @param[in]    mtx             mutex instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_mutex_t
 */
int pth_mtx_lock (pth_mutex_t *mtx);

/**
 *
 * @brief    Unlocks the Mutex for the given Caffeine Mutex Instance
 *
 * Unlocks the Mutex for the given Caffeine Mutex Instance.
 *
 * @param[in]    mtx             mutex instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_mutex_t
 */
int pth_mtx_unlock (pth_mutex_t *mtx);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_THREAD_MUTEX_H */
/* caf_thread_mutex.h ends here */

