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
#ifndef CAF_IPC_SHM_H
#define CAF_IPC_SHM_H 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <caf/caf_data_lstdl.h>

/**
 * @defgroup      caf_ipc_shm        IPC Shared Memmory
 * @ingroup       caf_ipc
 * @addtogroup    caf_ipc_shm
 * @{
 *
 * @brief     IPC Shared Memmory
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * IPC Shared Memmory
 *
 */

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#define CAF_SHM_POOL_SZ                 (sizeof (caf_shm_pool_t))
#define CAF_SHM_ALLOC_SZ                (sizeof (caf_shm_alloc_t))
#define CAF_SHM_DETACH_OK(s)            (s->ptr != (void *)-1)
#define CAF_SHM_BAD_ALLOC               ((void *)-1)

	typedef struct caf_shm_pool_s caf_shm_pool_t;
	struct caf_shm_pool_s {
		int count;
		size_t sz;
		lstdl_t *pool;
	};

	typedef struct caf_shm_alloc_s caf_shm_alloc_t;
	struct caf_shm_alloc_s {
		int id;
		int flags;
		key_t key;
		size_t sz;
		void *ptr;
	};

	caf_shm_pool_t *caf_shm_pool_new ();
	int caf_shm_pool_delete (caf_shm_pool_t *s);
	int caf_shm_pool_add (caf_shm_pool_t *s, caf_shm_alloc_t *a);

	caf_shm_alloc_t *caf_shm_seg_new (key_t k, size_t sz, int flg);
	int caf_shm_seg_delete (caf_shm_alloc_t *s);
	void *caf_shm_seg_attach (caf_shm_alloc_t *s);
	int caf_shm_seg_detach (caf_shm_alloc_t *s);
	int caf_shm_seg_ctrl (caf_shm_alloc_t *s, int cmd, struct shmid_ds *buf);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_IPC_SHM_H */
/* caf_ipc_shm.h ends here */
