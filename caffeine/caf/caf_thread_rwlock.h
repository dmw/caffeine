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
#ifndef CAF_THREAD_RWLOCK_H
#define CAF_THREAD_RWLOCK_H 1

#include <pthread.h>
#include <time.h>

/**
 * @defgroup      caf_thread_rwlock    Thread Read/Write Locks
 * @ingroup       caf_thread
 * @addtogroup    caf_thread_rwlock
 * @{
 *
 * @brief     Thread Read/Write Locks.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 *
 *
 */

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

/** Defines the pth_rwlock_t structure size */
#define CAF_PTH_RWLOCK_T_SZ            sizeof(pth_rwlock_t)
/** Defines the flag with all RWLOCKS set */
#define CAF_ALL_RWLOCKS                (PTHDR_RWLOCK_RDLOCK | \
                                        PTHDR_RWLOCK_WRLOCK | \
                                        PTHDR_RWLOCK_TRDLOCK | \
                                        PTHDR_RWLOCK_TWRLOCK)

/**
 *
 * @brief    Defines the pth_rwlock_sets attribute properties.
 * Defines the types that can be used with pth_rwlock_t structures.
 */
typedef enum {
    /** pthread_rwlockattr_(get|set)pshared */
    PTHDR_RWLOCK_PSHARED = 0000001,
    /** pthread_rwlockattr_(get|set)pshared */
    PTHDR_RWLOCK_PPRIVATE = 0000002,
    /** read lock */
    PTHDR_RWLOCK_RDLOCK = 0000004,
    /** write lock */
    PTHDR_RWLOCK_WRLOCK = 0000010,
    /** timed read lock */
    PTHDR_RWLOCK_TRDLOCK = 0000020,
    /** timed write lock */
    PTHDR_RWLOCK_TWRLOCK = 0000040
} pth_rwlock_types_t;

/**
 *
 * @brief    Caffeine Thread RWLock Type.
 * The type of a thread rwlock.
 * @see      pth_rwlock
 */
typedef struct pth_rwlock_s pth_rwlock_t;
/**
 *
 * @brief    Caffeine Thread RWLock Structure.
 * Stores the identifier of a RWLock, the attribute flags, the lock attributes
 * and the rwlock structure, allowing the right use of the rwlock information
 * through the program.
 */
struct pth_rwlock_s {
    /** Lock identifier */
    int id;
    /** Lock Attributes Flags */
    int at;
    /** Mutex Attributes */
    pthread_rwlockattr_t attr;
    /** The Thread Mutex */
    pthread_rwlock_t rwlock;
};

/**
 *
 * @brief    Creates a new Caffeine Thread Mutex Instance.
 *
 * Allocates memory for a pth_rwlock_t structure.
 *
 * @return       pth_rwlock_t *     the allocated pth_rwlock_t structure.
 *
 * @see      pth_rwlock_t
 */
pth_rwlock_t *pth_rwl_new (int id);

/**
 *
 * @brief    Deletes a new Caffeine Thread Mutex Instance.
 *
 * Deallocates memory for a pth_rwlock_t structure.
 *
 * @param[in]    rwl               pointer to the allocated rwlock.
 * @return       pth_rwlock_t *     the allocated pth_rwlock_t structure.
 *
 * @see      pth_rwlock_t
 */
void pth_rwl_delete (pth_rwlock_t *rwl);

/**
 *
 * @brief    Inits the Caffeine Mutex Attributes on the given Mutex Instance
 *
 * Initis the rwlock attributes for the given rwlock instance.
 *
 * @param[in]    rwl             rwlock instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_rwlock_t
 */
int pth_rwlattr_init (pth_rwlock_t *rwl);

/**
 *
 * @brief    Destroys the Caffeine Mutex Attributes on the given Mutex Instance
 *
 * Destroys the rwlock attributes for the given rwlock instance.
 *
 * @param[in]    rwl             rwlock instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_rwlock_t
 */
int pth_rwlattr_destroy (pth_rwlock_t *rwl);

/**
 *
 * @brief    Initializes the rwlock in the given Caffeine Mutex Instance
 *
 * Initializes the rwlock in the given Caffeine Mutex Instance.
 *
 * @param[in]    rwl             rwlock instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_rwlock_t
 */
int pth_rwl_init (pth_rwlock_t *rwl);

/**
 *
 * @brief    Destroys the rwlock in the given Caffeine Mutex Instance
 *
 * Initializes the rwlock in the given Caffeine Mutex Instance.
 *
 * @param[in]    rwl             rwlock instance to init attributes.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_rwlock_t
 */
int pth_rwl_destroy (pth_rwlock_t *rwl);

/**
 *
 * @brief    Sets an attribute for the given Caffeine Mutex Instance
 *
 * Sets an attribute for the given Caffeine Mutex Instance, with a type
 * of attribute and atrribute value.
 *
 * @param[in]    rwl             rwlock instance to init attributes.
 * @param[in]    t               attribute type.
 * @param[in]    data            data to use with the rwlock.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_rwlock_t
 */
int pth_rwlattr_set (pth_rwlock_t *rwl, pth_rwlock_types_t t, int data);

/**
 *
 * @brief    Gets an attribute for the given Caffeine Mutex Instance
 *
 * Gets an attribute for the given Caffeine Mutex Instance, with a type
 * of attribute and atrribute value.
 *
 * @param[in]    rwl             rwlock instance to init attributes.
 * @param[in]    t               attribute type.
 * @param[out]   data            pointer to store attribute data.
 * @return       int             zero on success, or an error code.
 *
 * @see      pth_rwlock_t
 */
int pth_rwlattr_get (pth_rwlock_t *rwl, pth_rwlock_types_t t, int *data);
int pth_rwl_wrlock (pth_rwlock_t *rwl, int tl, const struct timespec *to);
int pth_rwl_rdlock (pth_rwlock_t *rwl, int tl, const struct timespec *to);
int pth_rwl_unlock (pth_rwlock_t *rwl);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_THREAD_RWLOCK_H */
/* caf_thread_rwlock.h ends here */
