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
#ifndef CAF_DATA_BUFFER_H
#define CAF_DATA_BUFFER_H 1

#include <sys/types.h>
#include <caf/caf_data_lstdl.h>

/**
 * @defgroup      caf_data_buffer    Data Buffer
 * @ingroup       caf_data_string
 * @addtogroup    caf_data_buffer
 * @{
 *
 * @brief     Caffeine Data Buffer
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine Data Buffer functions allows you to store data in the
 * cbuffer_t structure, with two members inside, the void pointer
 * to the buffer allocated memory and the buffer size. This allows
 * some basicall operations such as creation, importing, exporting,
 * cutting, pasting, slicing, and some other operations. This simplify
 * the common buffer operations through the cbuffer_t structure.
 *
 * This means you can treat buffers through this structure, improving
 * buffer operations and avoiding common code traps such as buffer
 * overflows -- the buffer size is given allways -- and the proper
 * evaluation of the buffer size permits the right treatment of the
 * buffers.
 *
 */
#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#ifndef CAF_BUFF_SZ
#define CAF_BUFF_SZ              (sizeof(cbuffer_t))
#endif /* !CAF_BUFFER_SZ */

#ifndef CAF_BUFF_DELETE_CB
#define CAF_BUFF_DELETE_CB(cb)   int (*cb)(void *ptr, size_t sz)
#endif /* !CAF_BUFF_DELETE_CB */

/**
 *
 * @brief    Caffeine buffer storage type.
 * The type of a buffer storage structure.
 * @see      caf_buffer
 */
typedef struct caf_buffer_s cbuffer_t;

/**
 *
 * @brief    Caffeine buffer structure.
 * Buffer structure with dynamic data pointer and size.
 * @see      cbuffer_t
 */
struct caf_buffer_s {
    /** Buffer size */
    size_t sz;
    /** I/O Size */
    ssize_t iosz;
    /** Data storage pointer */
    void *data;
};

/**
 *
 * @brief    Empty Buffer allocator.
 *
 * Creates a new cbuffer_t pointer, the pointer must be
 * destroyed if is created through this way, this avoids the use of
 * malloc() system call, setting up a new cbuffer_t empty and ready
 * to be used.
 *
 * @return   cbuffer_t *            a new allocated buffer.
 *
 * @see      cbuf_delete
 */
cbuffer_t *cbuf_new (void);

/**
 *
 * @brief    Buffer allocator.
 *
 * Creates a new cbuffer_t pointer, the pointer must be
 * destroyed if is created through this way, this avoids the use of
 * malloc() system call, setting up a new cbuffer_t clean and ready
 * to be used. The buffer size is equivalent to the given sz parameter.
 *
 * @return   cbuffer_t *            a new allocated buffer.
 *
 * @see      cbuf_delete
 */
cbuffer_t *cbuf_create (size_t sz);

/**
 *
 * @brief    Buffer destructor.
 *
 * Deletes a node a deallocate the memory used by the pointer
 * data, using the sz member to set this memory to zero.
 *
 * @param[in]        buf        the buffer to delete.
 * @see      cbuf_new
 */
void cbuf_delete (cbuffer_t *buf);

/**
 *
 * @brief    Buffer destructor in an interactive way.
 *
 * Deletes a node a deallocate the memory used by the pointer
 * data, using the sz member to set this memory to zero. This
 * is an interactive way to desatroy the data buffer, with the
 * given destroy callback, allows you to setup a buffer of
 * structures or unions and destroy the data buffer with the
 * given callback. The callback must return zero, or the allocated
 * structure will not be deallocated.
 *
 * @param[in]        buf             the buffer to delete.
 * @param[in]        cb              the callback function.
 * @see      cbuf_new
 */
int cbuf_delete_interactive (cbuffer_t *buf, CAF_BUFF_DELETE_CB(cb));

/**
 *
 * @brief    Default delete Callback.
 *
 * @param[in]        cbuf            the buffer to delete.
 */
int cbuf_delete_callback (void *cbuf);

/**
 *
 * @brief    Buffer cleaner.
 *
 * Sets the memory refered by the data pointer in the cbuffer_t
 * structure to zero or NULL.
 *
 * @param[in]        buf            the buffer to clean.
 *
 */
void cbuf_clean (cbuffer_t *buf);

/**
 *
 * @brief    Buffer copying function.
 *
 * Copy the data the source buffer to the destination buffer, using
 * the xmemcpy function. When no space is available in the destination
 * buffer, is reallocated to allow the complete copy. If the destination
 * buffer has already allocated memory, is reallocated to the source
 * size and the bytes are copied into.
 *
 * @param[in]        dst        the source buffer.
 * @param[out]       src        the destination buffer.
 * @return           amount of bytes copied.
 */
size_t cbuf_copy (cbuffer_t *dst, const cbuffer_t *src);

/**
 *
 * @brief    Buffer import function.
 *
 * Imports memory block into the given buffer. The memory block referenced
 * by data is copied into the dst buffer data pointer, in the amount of
 * sz bytes. The destination data pointer is reallocated to the sz bytes
 * of size.
 *
 * @param[out]       dst           the destination buffer.
 * @param[in]        data          the source data pointer.
 * @param[in]        sz            amount of bytes to copy.
 * @return           amount of bytes copied.
 */
size_t cbuf_import (cbuffer_t *dst, const void *data, const size_t sz);

/**
 *
 * @brief    Extract a piece of the buffer.
 *
 * If the given source pointer is not null and the from and to parameters
 * are between 0 and the given source buffer size, creates a new cbuffer_t
 * pointer and copy the data into the new created buffer that is between
 * the from and to bytes in the source buffer.
 *
 * @param[out]       src        the source buffer.
 * @param[in]        from       from the position.
 * @param[in]        to         to the position.
 * @return           the sliced buffer.
 */
cbuffer_t *cbuf_extract (const cbuffer_t *src, const size_t from,
                         const size_t to);

/**
 *
 * @brief    Cut a piece of the buffer.
 *
 * Cut a piece of the given buffer. This means, that the sliced piece
 * of buffer is removed from the source buffer, the rest of the buffer
 * is positioned where the slice begins. The unused buffer piece is set
 * to NULL and removed using the realloc() system call.
 *
 * This function assumes that both memmove() and realloc() does not
 * destroys the pointed data.
 *
 * @param[out]       src           the source buffer.
 * @param[in]        from          from the position.
 * @param[in]        to            to the position.
 * @return           the sliced buffer.
 */
cbuffer_t *cbuf_cut (cbuffer_t *src, const size_t from, const size_t to);

/**
 *
 * @brief    Paste a buffer piece into a cbuffer_t pointer.
 *
 * Paste a piece of the given buffer, into the position from_dst,
 * filling from this position with the sice of the source slice,
 * starting at from_src and ending in to_src.
 *
 * @param[out]       dst            destination buffer.
 * @param[in]        src            source buffer.
 * @param[in]        from_dst       startup position in destination.
 * @param[in]        from_src       startup position in source.
 * @param[in]        to_src         end position in destination.
 * @return           the sliced buffer.
 */
cbuffer_t *cbuf_paste (cbuffer_t *dst, const cbuffer_t *src,
                       const size_t from_dst, const size_t from_src,
                       const size_t to_src);

/**
 *
 * @brief    Splits a Caffeine Buffer into pieces and put them in DLL.
 *
 * Splits a Caffeine Buffer into pieces using the pattern to separate
 * the Buffer. The splitted pieces fills a Caffeine Double Linked List.
 *
 * @param[out]       src            the source buffer.
 * @param[in]        pattern        the divisor pattern.
 * @return           a DLL with the pieces.
 */
lstdl_t *cbuf_split (cbuffer_t *src, const void *pattern, size_t patsz);

/**
 *
 * @brief    Joins a DLL of Caffeine Buffers.
 *
 * Joins a DLL (Double Linked List) of Caffeine Buffers into one Buffer.
 *
 * @param[out]       lst            the list containings the buffers.
 * @return           a Caffeine Buffer with the joined buffers.
 */
cbuffer_t *cbuf_join (lstdl_t *lst);

/**
 *
 * @brief    Search Data in Buffers.
 *
 * Searches for Data in Buffers, returning a list containing the pointers
 * to the search ocurrences. The list must be deleted with lstdl_delete_nocb
 * function, because any pointer in the list can not be free.
 *
 * @param[in]        src            the buffer to modify.
 * @param[in]        srch           data to search.
 * @param[in]        srchsz         size of data to search.
 * @return           a new created buffer with replaced data.
 *
 * @see lstdl_delete_nocb
 */
lstdl_t *cbuf_search (cbuffer_t *src, void *srch, size_t srchsz);

/**
 *
 * @brief    Replaces Data in Buffers.
 *
 * Replaces Data in Caffeine Buffers, using the given buffer, a search
 * data pointer with a search data size, that will be replaced by the
 * data contained in another data pointer with a given size.
 *
 * @param[in]        src            the buffer to modify.
 * @param[in]        srch           data to search.
 * @param[in]        repl           data to replace.
 * @param[in]        srchsz         size of data to search.
 * @param[in]        replsz         replacement size.
 * @return           a new created buffer with replaced data.
 */
cbuffer_t *cbuf_replace (cbuffer_t *src, void *srch, void *repl,
                         size_t srchsz, size_t replsz);

/**
 *
 * @brief    Extracts the Head of a Buffer.
 *
 * Gets the head of a buffer using the cbuf_extract function.
 *
 * @param[in]        src            the buffer to modify.
 * @param[in]        sz             size of data to search.
 * @return           a new created buffer with the head of the buffer.
 *
 * @see cbuf_extract
 */
cbuffer_t *cbuf_head (cbuffer_t *src, size_t sz);

/**
 *
 * @brief    Extracts the Tail of a Buffer.
 *
 * Gets the tail of a buffer using the cbuf_extract function.
 *
 * @param[in]        src            the buffer to modify.
 * @param[in]        sz             size of data to search.
 * @return           a new created buffer with the head of the buffer.
 *
 * @see cbuf_extract
 */
cbuffer_t *cbuf_tail (cbuffer_t *src, size_t sz);

/**
 *
 * @brief    Extracts the Head of a Buffer cutting it.
 *
 * Gets the head of a buffer using the cbuf_cut function.
 *
 * @param[in]        src            the buffer to modify.
 * @param[in]        sz             size of data to search.
 * @return           a new created buffer with the head of the buffer.
 *
 * @see cbuf_cut
 */
cbuffer_t *cbuf_head_cut (cbuffer_t *src, size_t sz);

/**
 *
 * @brief    Extracts the Tail of a Buffer cutting it.
 *
 * Gets the tail of a buffer using the cbuf_cut function.
 *
 * @param[in]        src            the buffer to modify.
 * @param[in]        sz             size of data to search.
 * @return           a new created buffer with the head of the buffer.
 *
 * @see cbuf_cut
 */
cbuffer_t *cbuf_tail_cut (cbuffer_t *src, size_t sz);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_BUFFER_H */
/* caf_data_buffer.h ends here */
