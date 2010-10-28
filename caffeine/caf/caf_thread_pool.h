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
#ifndef CAF_THREAD_POOL_H
#define CAF_THREAD_POOL_H 1

#include <pthread.h>
#include <caf/caf_data_deque.h>

/**
 * @defgroup      caf_thread_pool    Thread Pool Manager
 * @ingroup       caf_thread
 * @addtogroup    caf_thread_pool
 * @{
 *
 * @brief     Thread Pool Manager.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@coder.cl>
 *
 * Thread Pool Manager is the set of functions to manage a thread pool.
 * A Thread Pool is a DLL of threads that shares the same attributes,
 * locks and conditions.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Defines the pth_pool_t size */
#define CAF_PT_POOL_SZ             sizeof(pth_pool_t)
/** Defines the thread routin prototype */
#define CAF_PT_PROTOTYPE(rtn)      void *(*rtn)(void *)

/**
 *
 * @brief    Caffeine Thread Pool Type.
 * The type of a thread pool.
 * @see      pth_pool
 */
typedef struct pth_pool_s pth_pool_t;
/**
 *
 * @brief    Caffeine Thread Pool Structure.
 * Structure designed to hold information about a pool of thread, that shares
 * common attributes and the same thread routine. The purpose is to create
 * a thread pool to build a service oriented pool.
 */
struct pth_pool_s {
	/** Thread Count */
	int count;
	/** Thread Attributes */
	pth_attri_t *attri;
	/** Thread List */
	deque_t *threads;
	/** Thread Routine */
	CAF_PT_PROTOTYPE(rtn);
};


/**
 *
 * @brief    Allocates a new Caffeine Thread Pool Instance.
 *
 * Allocates memory for a pth_pool_t structure, setting the attributes of
 * the pool threads, the common routine to all threads and the amount
 * of threads to launch.
 *
 * @param[in]    attrs           Caffeine Thread Attributes.
 * @param[in]    rtn             thread routine.
 * @param[in]    count           number of threads to launch.
 * @return       pth_pool_t *    the allocated pth_pool_t structure.
 *
 * @see      pth_pool_t
 */
pth_pool_t *pth_pool_new (pth_attri_t *attrs,
						  CAF_PT_PROTOTYPE(rtn), int count);

/**
 *
 * @brief    Deletes a Caffeine Thread Pool Instance.
 *
 * Deallocates memory for a pth_pool_t structure.
 *
 * @param[in]    p               the pool to delete.
 *
 * @see      pth_pool_t
 */
void pth_pool_delete (pth_pool_t *p);

/**
 *
 * @brief    Callback to use with the deletion of the thread list in the pool.
 *
 * Callback to use with the deletion of the thread list in the pool.
 *
 * @param[in]    ptr             pointer to the routine thread structure.
 * @return       int             one on success, zero on error.
 *
 * @see      pth_pool_t
 */
int pth_pool_delete_callback (void *ptr);

/**
 *
 * @brief    Creates a new Pool.
 *
 * Do all the job to create a thread pool with a common task, ideal for single
 * services.
 *
 * @param[in]    attrs           Caffeine Thread Attributes.
 * @param[in]    rtn             thread routine.
 * @param[in]    cnt             number of threads to launch.
 * @param[in]    arg             thread routine arguments.
 * @return       pth_pool_t *    the allocated and working pool.
 *
 * @see      pth_pool_t
 */
pth_pool_t *pth_pool_create (pth_attri_t *attrs,
							 CAF_PT_PROTOTYPE(rtn), int cnt, void *arg);

/**
 *
 * @brief    Adds a Thread to the Pool.
 *
 * Adds threads to the given pool.
 *
 * @param[in]    p               Caffeine Thread Pool.
 * @param[in]    attrs           Caffeine Thread Attributes.
 * @param[in]    rtn             Thread Routine.
 * @return       int             zero on success.
 *
 * @see      pth_pool_t
 */
int pth_pool_add (pth_pool_t *p, pth_attri_t *attrs, CAF_PT_PROTOTYPE(rtn),
				  void *arg);

/**
 *
 * @brief    Joins the entire given pool.
 *
 * Joins the all the threads in the given thread pool.
 *
 * @param[in]    pool            Caffeine Thread Pool.
 * @return       int             zero on success, upper to zero on error.
 *
 * @see      pth_pool_t
 */
int pth_pool_join (pth_pool_t *pool);

/**
 *
 * @brief    Detaches the entire given pool.
 *
 * Detaches the all the threads in the given thread pool.
 *
 * @param[in]    pool            Caffeine Thread Pool.
 * @return       int             zero on success, upper to zero on error.
 *
 * @see      pth_pool_t
 */
int pth_pool_detach (pth_pool_t *pool);

/**
 *
 * @brief    Cancels the entire given pool.
 *
 * Cancels the all the threads in the given thread pool.
 *
 * @param[in]    pool            Caffeine Thread Pool.
 * @return       int             zero on success, upper to zero on error.
 *
 * @see      pth_pool_t
 */
int pth_pool_cancel (pth_pool_t *pool);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_THREAD_POOL_H */
/* caf_thread_pool.h ends here */

