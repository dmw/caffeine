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
#ifndef CAF_SEM_POSIX_H
#define CAF_SEM_POSIX_H 1

#include <stdlib.h>
#include <stdio.h>

#include <semaphore.h>

/**
 * @defgroup      caf_sem_posix        POSIX Semaphores
 * @ingroup       caf_sem
 * @addtogroup    caf_sem_posix
 * @{
 *
 * @brief     POSIX Semaphores.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@coder.cl>
 *
 * POSIX Semaphores.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define CAF_SEM_POSIX_SZ                (sizeof (caf_sem_posix_t))

typedef struct caf_sem_posix_s caf_sem_posix_t;
struct caf_sem_posix_s {
	sem_t key;
	int flag;
	int value;
};

caf_sem_posix_t *caf_sem_posix_new (const sem_t key, const int flag,
									const int value);
int caf_sem_posix_delete (caf_sem_posix_t *r);
caf_sem_posix_t *caf_sem_posix_get (const sem_t key, const int flag,
									const int value);

int caf_sem_posix_lock (caf_sem_posix_t *r);
int caf_sem_posix_trylock (caf_sem_posix_t *r);
int caf_sem_posix_unlock (caf_sem_posix_t *r);

int caf_sem_posix_exists (caf_sem_posix_t *r);
int caf_sem_posix_post (caf_sem_posix_t *r);
int caf_sem_posix_getvalue (caf_sem_posix_t *r);
int caf_sem_posix_wait (caf_sem_posix_t *r);
int caf_sem_posix_trywait (caf_sem_posix_t *r);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_SEM_POSIX_H */
/* caf_sem_posix.h ends here */

