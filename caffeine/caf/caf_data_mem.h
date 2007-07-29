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
#ifndef CAF_DATA_MEM_H
#define CAF_DATA_MEM_H 1
/**
 * @defgroup      caf_memory    Memory Routines
 * @ingroup       caf_mem
 * @addtogroup    caf_memory
 * @{
 *
 * @brief     Caffeine Memory Routines.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Common memory routines, as defines the GNU Coding Standards, these
 * are wrapper routines to common memory allocation and deallocation
 * functions.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#define PTRISNULL(ptr)      (void *)ptr == (void *)NULL
#define PTRNOTNULL(ptr)     (void *)ptr != (void *)NULL
#define STRSZ(ptr)          (size_t)((strlen (ptr)))
#define PRTINIT(ptr)        (void *)ptr == (void *)NULL

#ifndef CAFFEINE_DEBUG

/**
 *
 * @brief    Allocates memory.
 *
 * Creates a new void pointer with allocated mem bytes.
 *
 * @param[in]    size_t         ammount of memory to allocate.
 * @return       void *         the allocated pointer.
 *
 * @see      xfree
 * @see      xdestroy
 */
void *xmalloc (size_t mem);

/**
 *
 * @brief    Empties a block of memory.
 *
 * Sets a block of memory to zero, setting all data in the given
 * size to zero pointed from the given pointer.
 *
 * @param[in]    void *         the pointer to set to zero (NULL).
 * @param[in]    size_t         the amount of memory to set to zero.
 * @return       void
 *
 */
void xempty (void *ptr, size_t sz);

/**
 *
 * @brief    Free the given allocated pointer.
 *
 * Free the given allocated pointer. The given pointer must be
 * an allocated pointer by the xmalloc function.
 *
 * @param[in]    void *         the pointer to free.
 * @return       void
 *
 */
void xfree (void *ptr);

/**
 *
 * @brief    Destroys the given pointer.
 *
 * Free the allocated the given allocated pointer and sets the
 * the memory referenced by the pointer -- with the given size -- to
 * zero.
 *
 * @param[in]    void *         the pointer to free.
 * @param[in]    size_t         the ammount of memory to set to null.
 * @return       void
 *
 */
void xdestroy (void *ptr, size_t sz);

/**
 *
 * @brief    Reallocates memory for a pointer.
 *
 * Reallocates memory for a pointer, in a non destructive way,
 * this means the data previouslly allocated is untouched.
 *
 * @param[in]    void *         the pointer to realloc.
 * @param[in]    size_t         the ammount of memory to reallocate.
 * @return       void *
 *
 */
void *xrealloc (void *ptr, size_t sz);

/**
 *
 * @brief    Destroys a string (char *).
 *
 * Deallocates memory for a pointer, in a destructive way,
 * this means the data previouslly allocated is set to zero.
 *
 * @param[in]    void *         the string pointer to free.
 * @return       void
 *
 */
void xstrdestroy (void *ptr);

/**
 *
 * @brief    Copies memory from one pointer to another.
 *
 * Copies memory from the given src pointer to the given dst pointer
 * the ammount of memory indicated by sz. Returns the destination
 * pointer on success, as defines the memcpy standard.
 *
 * @param[in]    void *         the destination pointer.
 * @param[in]    void *         the source pointer.
 * @param[in]    size_t         the ammount of memory to copy.
 * @return       void
 *
 */
void *xmemcpy (void *dst, void *src, size_t sz);

#else /* use debugging functions */

#define xmalloc(mem) \
        malloc (mem)
#define xempty(ptr,sz) \
        memset (ptr, 0, sz)
#define xfree(ptr) \
        free (ptr)
#define xdestroy(ptr,sz) \
        memset (ptr, 0, sz); \
        free (ptr)
#define xrealloc(ptr,sz) \
        realloc (ptr, sz)
#define xstrdestroy(ptr) \
        memset (ptr, 0, strlen (ptr)); \
        free (ptr)
#define xmemcpy(dst,src,sz) \
        memcpy (dst, src, sz)

#endif /* !CAFFEINE_DEBUG */

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_MEM_H */
/* caf_data_mem.h ends here */
