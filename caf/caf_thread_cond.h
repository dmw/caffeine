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
#ifndef CAF_THREAD_COND_H
#define CAF_THREAD_COND_H 1

#include <sys/types.h>
#include <pthread.h>
#include <caf/caf_thread_mutex.h>
#include <caf/caf_data_deque.h>

/**
 * @defgroup      caf_thread_cond    Thread Condition Variables
 * @ingroup       caf_thread
 * @addtogroup    caf_thread_cond
 * @{
 *
 * @brief     Thread Condition Variables
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@coder.cl>
 *
 * Manage Thread Condition Variables.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define PTH_COND_SZ                     (sizeof (pth_cond_t))

typedef struct pth_cond_s pth_cond_t;
struct pth_cond_s {
	int at;
	pthread_condattr_t attr;
	pthread_cond_t cond;
};


pth_cond_t *pth_cond_new (void);
int pth_cond_delete (pth_cond_t *c);
int pth_cond_init (pth_cond_t *c);
int pth_cond_destroy (pth_cond_t *c);
int pth_condattr_init (pth_cond_t *c);
int pth_condattr_destroy (pth_cond_t *c);
pth_cond_t *pth_condi_init (void);
int pth_condi_delete (pth_cond_t *c);
int pth_cond_signal (pth_cond_t *c);
int pth_cond_broadcast (pth_cond_t *c);
int pth_cond_wait (pth_cond_t *c, pth_mutex_t *m);
int pth_cond_timedwait (pth_cond_t *c, pth_mutex_t *m,
						const struct timespec *tm);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_THREAD_COND_H */
/* caf_thread_cond.h ends here */

