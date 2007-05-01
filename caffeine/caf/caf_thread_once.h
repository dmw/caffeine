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

    $Id: caf_thread_once.h 198 2007-02-15 10:13:36Z dmw $
 */
#ifndef CAF_THREAD_ONCE_H
#define CAF_THREAD_ONCE_H 1

#include <pthread.h>

/**
 * @defgroup      caf_thread_once    Thread Once Control
 * @ingroup       caf_thread
 * @addtogroup    caf_thread_once
 * @{
 *
 * @brief     Thread Once Control.
 * @date      $Date: 2007-02-15 07:13:36 -0300 (Thu, 15 Feb 2007) $
 * @version   $Revision: 198 $
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Manage the Thread Once executed routines.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

/** Defines the Thread Once routine prototype */
#define PTH_ONCE_RTN(rtn)               void (*rtn)(void)
/** Defines the pth_once_t size */
#define PTH_ONCE_SZ                     sizeof(pth_once_t)

/**
 *
 * @brief    Caffeine Thread Once Type.
 * The type of a thread once structure.
 * @see      pth_once
 */
typedef struct pth_once_s pth_once_t;
/**
 *
 * @brief    Caffeine Thread Once Structure.
 * This structure holds the once initiator and the pointer the once
 * executed routine by the thread.
 */
struct pth_once_s {
    /** Once initiator */
    pthread_once_t once;
    /** Once Executed Routine */
    PTH_ONCE_RTN(rtn);
};

/**
 *
 * @brief    Creates a new Caffeine Thread Once structure.
 *
 * Allocates memory for a pth_once_t structure.
 *
 * @param[in]    rtn               once routine.
 * @return       pth_once_t *      the allocated pth_once_t structure.
 *
 * @see      pth_once_t
 */
pth_once_t *pth_once_new (PTH_ONCE_RTN(rtn));

/**
 *
 * @brief    Deletes a Caffeine Thread Once structure.
 *
 * Deallocates memory for a pth_once_t structure.
 *
 * @param[in]    onc               once routine.
 * @return       pth_once_t *      the allocated pth_once_t structure.
 *
 * @see      pth_once_t
 */
void pth_once_delete (pth_once_t *onc);

/**
 *
 * @brief    Sets the call once for the Current Thread.
 *
 * Sets the call once for the current thread.
 *
 * @param[in]    call              the pth_once_t pointer.
 * @return       int               zero on success, error code on failure.
 *
 * @see      pth_once_t
 */
int pth_once_call (pth_once_t *call);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_THREAD_ONCE_H */
/* caf_thread_once.h ends here */
