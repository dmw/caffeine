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
#ifndef CAF_IO_NET_SVCPOOL_H
#define CAF_IO_NET_SVCPOOL_H 1
/**
 * @defgroup      caf_io_net_svcpool                Netowork Service Pool
 * @ingroup       caf_io
 * @addtogroup    caf_io_net_svcpool
 * @{
 *
 * @brief     Network I/O
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Netowork Service Pool
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#include <caf/caf_io_net.h>

#define CAF_SVCPOOL_SZ              (sizeof (caf_svcpool_t))

typedef struct caf_svcpool_s caf_svcpool_t;
struct caf_svcpool_s {
	int svc_id;
	int svc_num;
	int *svc_fds;
	caf_conn_t *svc_seed;
	lstdl_t *svc_lst;
};

caf_svcpool_t *caf_svcpool_new (int id, int num, caf_conn_t *seed);
int caf_svcpool_delete (caf_svcpool_t *svc);
int caf_svcpool_init (caf_svcpool_t *svc);
int caf_svcpool_stop (caf_svcpool_t *svc);
int caf_svcpool_close (caf_svcpool_t *svc);
int caf_svcpool_finalize (caf_svcpool_t *svc);
int caf_svcpool_reopen (caf_svcpool_t *svc);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_IO_NET_SVCPOOL_H */
/* caf_io_net_svcpool.h ends here */
