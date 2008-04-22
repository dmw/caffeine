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
#ifndef CAF_SEM_SVR4_H
#define CAF_SEM_SVR4_H 1

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <caf/caf_data_lstdl.h>

/**
 * @defgroup      caf_sem_svr4        SVr4 Semaphores
 * @ingroup       caf_sem
 * @addtogroup    caf_sem_svr4
 * @{
 *
 * @brief     Caffeine SVr4 Semaphores.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine SVr4 Semaphores.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define CAF_SEM_SVR4_SZ                 (sizeof (caf_sem_svr4_t))
#define CAF_SEM_BUF_SZ                  (sizeof (struct sembuf))
#define CAF_SEMBUF_SVR4_SZ              (sizeof (caf_sembuf_svr4_t))

typedef enum {
	CAF_SEM_SVR4_LOCK = -0x0001,
	CAF_SEM_SVR4_UNLOCK = 0x0001
} caf_sem_svr4_lock_t;

typedef struct caf_sem_svr4_s caf_sem_svr4_t;
struct caf_sem_svr4_s {
	key_t key;
	int id;
	int nsems;
	int flag;
};

typedef struct caf_sembuf_svr4_s caf_sembuf_svr4_t;
struct caf_sembuf_svr4_s {
	size_t nopsz;
	struct sembuf *ops;
	struct timespec *to;
};

caf_sem_svr4_t *caf_sem_svr4_new (const key_t key, const int nsems,
								  const int flag);
int caf_sem_svr4_delete (caf_sem_svr4_t *r);
caf_sem_svr4_t *caf_sem_svr4_get (const key_t k, const int ns, const int f);

int caf_sem_svr4_lock (caf_sem_svr4_t *r);
int caf_sem_svr4_trylock (caf_sem_svr4_t *r);
int caf_sem_svr4_unlock (caf_sem_svr4_t *r);

int caf_sem_svr4_exists (caf_sem_svr4_t *r);
int caf_sem_svr4_op (caf_sem_svr4_t *r, caf_sembuf_svr4_t *op);
int caf_sem_svr4_ctl (caf_sem_svr4_t *r, int n, int cmd, void *a);
int caf_sem_svr4_down (caf_sem_svr4_t *r);
caf_sembuf_svr4_t *caf_sembuf_svr4_new (caf_sem_svr4_t *r, const short op,
										const short flg, struct timespec *to);
int caf_sembuf_svr4_delete (caf_sembuf_svr4_t *b);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_SEM_SVR4_H */
/* caf_sem_svr4.h ends here */

