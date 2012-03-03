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
#ifndef CAF_IPC_SHM_H
#define CAF_IPC_SHM_H 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <caf/caf_data_deque.h>

/**
 * @defgroup      caf_ipc_shm        IPC Shared Memmory
 * @ingroup       caf_ipc
 * @addtogroup    caf_ipc_shm
 * @{
 *
 * @brief     IPC Shared Memmory
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@coder.cl>
 *
 * IPC Shared Memmory
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Size of the <b>caf_shm_pool_t</b> structure */
#define CAF_SHM_POOL_SZ                 (sizeof (caf_shm_pool_t))
/** Size of the <b>caf_shm_alloc_t</b> structure */
#define CAF_SHM_ALLOC_SZ                (sizeof (caf_shm_alloc_t))
/** Evaluates if the given <b>caf_shm_alloc_t</b> structure <b>s</b>
  has a pointer targeting the allocation to <b>NULL</b> */
#define CAF_SHM_DETACH_OK(s)            (s->ptr != (void *)NULL)
/** Bad allocation points to <b>NULL</b>, because usally <b>NULL</b>
  points to the <i>zero</i> virtual address. */
#define CAF_SHM_BAD_ALLOC               ((void *)NULL)

/**
 * @brief SHM Allocation Block Type
 * C typedef for the <b>caf_shm_alloc_s</b> structure.
 *
 * @see caf_shm_alloc_s
 */
typedef struct caf_shm_alloc_s caf_shm_alloc_t;

/**
 * @brief SHM Allocation Block
 *
 * <p>This structure holds information about SHM allocation
 * blocks. Also has identifiers an identifier <b>id</b>,
 * allocation flags <b>flags</b>, ipc <b>key</b> of type
 * <b>key_t</b>, allocation size <b>sz</b> and allocation
 * pointer <b>ptr</b>.</p>
 *
 * <p>This structure is designed to work with the
 * <b>caf_shm_seg_*</b> interface set.
 *
 * @see caf_shm_alloc_t
 */
struct caf_shm_alloc_s {
	/** Block Identifier */
	int id;
	/** Block Flags (passed to <b>sgmget(2)</b>) */
	int flags;
	/** IPC Key */
	key_t key;
	/** Allocation Size (in bytes) */
	size_t sz;
	/** Allocation Pointer */
	void *ptr;
};

/**
 * @brief Pool of SHM Allocation Blocks Typedef
 * Pool of SHM Allocation Blocks Typedef.
 *
 * @see caf_shm_pool_s
 */
typedef struct caf_shm_pool_s caf_shm_pool_t;

/**
 * @brief Pool of SHM Allocation Blocks Structure
 * Holds a list of <b>caf_shm_alloc_t</b> structures, mantaining
 * a count of elements in the list and common allocation block
 * size.
 *
 * @see caf_shm_pool_t
 * @see caf_shm_alloc_t
 * @see caf_shm_alloc_s
 */
struct caf_shm_pool_s {
	/** Item Count */
	int count;
	/** Common Allocation Size */
	size_t sz;
	/** List of Blocks */
	deque_t *pool;
};

/**
 * @brief Allocates a new SHM Pool
 *
 * Allocates a new SHM Pool, with an empty list, ready to
 * add SHM allocations <b>caf_shm_alloc_t</b>.
 *
 * @return caf_shm_pool_t *		a new allocated SHM pool
 */
caf_shm_pool_t *caf_shm_pool_new ();

/**
 * @brief Deallocates a SHM Pool
 *
 * Deallocates a SHM Poll, this means deallocation of the
 * <b>deque_t</b> list of <b>caf_shm_alloc_t</b> structures.
 *
 * @param s[in]			pool to deallocate
 *
 * @return int			CAF_OK on success, CAF_ERROR on failure
 */
int caf_shm_pool_delete (caf_shm_pool_t *s);

/**
 * @brief Adds an allocation block to the given pool
 *
 * Adds the given allocation block structure <b>a</b> to
 * the given SHM pool <b>s</b>, incrementing the pool
 * size <b>s->sz</b> and the pool count <b>s->count</b>.
 *
 * @param s[in]			SHM Pool
 * @param a[in]			allocation block
 *
 * @return[int]			CAF_OK on success, CAF_ERROR on failure
 */
int caf_shm_pool_add (caf_shm_pool_t *s, caf_shm_alloc_t *a);

/**
 * @brief Creates a new allocation block
 *
 * Creates (allocate) a new allocation block storing data
 * about the allocation block in the returned structure
 * pointer <b>caf_shm_alloc_t</b>.
 *
 * @param k[in]			IPC key
 * @param sz[in]		block size
 * @param flg[in]		<b>shmget(2)</b> flags
 *
 * @return caf_shm_alloc_t *		new pointer, NULL on failure
 */
caf_shm_alloc_t *caf_shm_seg_new (key_t k, size_t sz, int flg);

/**
 * @brief Deletes an allocation block
 *
 * Deletes (deallocates) the given allocation block <b>s</b>.
 *
 * @param s[in]			the allocation block to delete
 *
 * @return int			CAF_OK on success, CAF_ERROR on failure
 */
int caf_shm_seg_delete (caf_shm_alloc_t *s);

/**
 * @brief Attach a pointer form the given allocation block
 *
 * <p>
 * This interface attaches the SHM segment to the calling
 * process.</p>
 * <p>
 * Attaches a segment (block) of memory from the given
 * allocation block created through <b>shmget(2)</b> interface
 * using the <b>caffeine</b> interface <b>caf_shm_seg_new</b>.</p>
 *
 * @param s[in]			allocation block from where attach a segment
 *
 * @return void *		a pointer to the block
 */
void *caf_shm_seg_attach (caf_shm_alloc_t *s);

/**
 * @brief Detaches memory from the given allocation block
 *
 * <p>
 * This interface deattaches the SHM segment to the calling
 * process.</p>
 * <p>
 * Dettaches a segment (block) of memory from the given
 * allocation block created through <b>shmget(2)</b> interface
 * using the <b>caffeine</b> interface <b>caf_shm_seg_new</b>.</p>
 *
 * @param s[in]			allocation block from where dettach
 *
 * @return int			CAF_OK on success, CAF_ERROR on failure
 */
int caf_shm_seg_detach (caf_shm_alloc_t *s);

/**
 * @brief Caffeine interface to the <b>shmctl(2)</b> system call.
 *
 * Operates over the given allocation block <b>s</b> using the
 * system call <b>shmctl(2)</b>. For further information read the
 * manual page of <b>shmctl(2)</b>.
 *
 * @param s[in]			allocation block to work on
 * @param cmd[in]		<b>shmctl(2)</b> command
 * @param buf[in]		<b>shmctl(2)</b> operation
 *
 * @return int	the result of the <b>shmctl(2)</b> interface
 */
int caf_shm_seg_ctrl (caf_shm_alloc_t *s, int cmd, struct shmid_ds *buf);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_IPC_SHM_H */
/* caf_ipc_shm.h ends here */

