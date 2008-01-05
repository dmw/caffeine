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
#ifndef CAF_THREAD_ATTR_H
#define CAF_THREAD_ATTR_H 1

#include <pthread.h>
#include <caf/caf_data_lstdl.h>

/**
 * @defgroup      caf_thread_attr    Thread Attributes
 * @ingroup       caf_thread
 * @addtogroup    caf_thread_attr
 * @{
 *
 * @brief     Thread Attributes.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * A way to manage thread attributes in a simplified way, trying to build an
 * abstraction layer for thread attributes. Some systems may have some
 * diferences between implementations, this module should work for the
 * Caffeine Threads Layer.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Defines the pth_attri_t structure size */
#define CAF_PT_ATTRI_SZ            sizeof(pth_attri_t)
/** Defines the pth_pool_t structure size */
#define CAF_PT_POOL_SZ             sizeof(pth_pool_t)

/**
 *
 * @brief    Defines the pth_attri_t property types
 * Defines the types that can be used with pth_attri_t structures.
 */
typedef enum {
	/** pthread_attr_(set|get)detachstate */
	PTH_ATTR_DETACHED = 0000001,
	/** pthread_attr_(set|get)detachstate */
	PTH_ATTR_JOINABLE = 0000002,
	/** pthread_attr_(set|get)stacksize */
	PTH_ATTR_STACKSZ = 0000004,
	/** pthread_attr_(set|get)guardsize */
	PTH_ATTR_GUARDSZ = 0000010,
	/** pthread_attr_(set|get)schedparam */
	PTH_ATTR_SCHEDPARAM = 0000020,
	/** pthread_attr_(set|get)inheritsched */
	PTH_ATTR_INSCHEDPARAM = 0000040,
	/** pthread_attr_(set|get)schedpolicy */
	PTH_ATTR_SCHEDPOLICY = 0000100,
	/** pthread_attr_(set|get)scope */
	PTH_ATTR_SCOPE = 0000200
} pth_attr_types_t;

/**
 *
 * @brief    Caffeine Thread Attribute Type.
 * The type of a thread attibute.
 * @see      pth_attri
 */
typedef struct pth_attri_s pth_attri_t;
/**
 *
 * @brief    Caffeine Thread Attribute Structure.
 * The thread attribute structure stores a thread attribute structure and
 * an integer that have the flags of the attributes that has been set in
 * the attr structure member.
 */
struct pth_attri_s {
	/** Flags of Attributes Set */
	int at;
	/** Thread Attributes */
	pthread_attr_t attr;
};

/**
 *
 * @brief    Creates a new Caffeine Attribute Instance.
 *
 * Allocates memory for a pth_attri_t structure, leaving the at memeber to
 * zero, this means that the pthread_attr_t has no attibutes set.
 *
 * @return       pth_attri_t *   the allocated pth_attri_t structure.
 *
 * @see      pth_attri_delete
 * @see      pth_attr_init
 * @see      pth_attr_delete
 */
pth_attri_t *pth_attri_new ();

/**
 *
 * @brief    Free am Attribute Instance.
 *
 * Deallocates memory for a pth_attri_t structure. Do not use before
 * pth_attr_destroy, or the pthread_attr_t in the structure will not
 * be destroyed.
 *
 * @param[in]    attri           pth_attri_t pointer.
 *
 * @see      pth_attri_new
 * @see      pth_attr_init
 * @see      pth_attr_destroy
 */
void pth_attri_delete (pth_attri_t *attri);

/**
 *
 * @brief    Inits the Attribute Instance.
 *
 * Calls the attibute initiation routine with the given pth_attri_t pointer.
 * Call this function after calling pth_attri_new.
 *
 * @param[in]    attri           pth_attri_t pointer.
 *
 * @see      pth_attri_new
 * @see      pth_attri_delete
 * @see      pth_attr_destroy
 */
int pth_attr_init (pth_attri_t *attri);

/**
 *
 * @brief    Inits the Attribute Instance.
 *
 * Calls the attibute destruction routine with the given pth_attri_t pointer.
 * Call this function before calling pth_attri_delete.
 *
 * @param[in]    attri           pth_attri_t pointer.
 *
 * @see      pth_attri_new
 * @see      pth_attri_delete
 * @see      pth_attr_init
 */
int pth_attr_destroy (pth_attri_t *attri);

/**
 *
 * @brief    Creates a new intitiated pth_attri_t pointer.
 *
 * Creates a new pth_attri_t initiated by pth_attr_init.
 *
 * @param[in]    attri           pth_attri_t pointer.
 *
 * @see      pth_attri_new
 * @see      pth_attri_delete
 * @see      pth_attr_init
 * @see      pth_attr_destroy
 * @see      pth_attri_destroy
 */
pth_attri_t *pth_attri_init ();

/**
 *
 * @brief    Destroy and deallocates the given pth_attri_t pointer.
 *
 * Calls pth_attr_destroy and then pth_attri_delete with the given
 * pth_attri_t pointer.
 *
 * @param[in]    attri           pth_attri_t pointer.
 *
 * @see      pth_attri_new
 * @see      pth_attri_delete
 * @see      pth_attr_init
 * @see      pth_attr_destroy
 * @see      pth_attri_init
 */
int pth_attri_destroy (pth_attri_t *attri);

/**
 *
 * @brief    Sets pth_attri_t instance properties.
 *
 * Sets the properties of the given pth_attri_t pointer (instance).
 *
 * @param[in]    attri           pth_attri_t pointer.
 * @param[in]    t               property type.
 * @param[in]    data            data to assign.
 *
 * @see      pth_attri_get
 */
int pth_attri_set (pth_attri_t *attri, pth_attr_types_t t, void *data);

/**
 *
 * @brief    Gets pth_attri_t instance properties.
 *
 * Gets the properties of the given pth_attri_t pointer (instance).
 *
 * @param[in]    attri           pth_attri_t pointer.
 * @param[in]    t               property type.
 * @param[in]    data            data to assign.
 *
 * @see      pth_attri_set
 */
int pth_attri_get (pth_attri_t *attri, pth_attr_types_t t, void *data);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_THREAD_ATTR_H */
/* caf_thread_attr.h ends here */

