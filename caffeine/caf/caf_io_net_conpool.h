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
#ifndef CAF_IO_NET_CONPOOL_H
#define CAF_IO_NET_CONPOOL_H 1
/**
 * @defgroup      caf_io_net_conpool                Netowork Connection Pool
 * @ingroup       caf_io
 * @addtogroup    caf_io_net_conpool
 * @{
 *
 * @brief     Netowork Connection Pool
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Netowork Connection Pool
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#include <caf/caf_io_net.h>

#define CAF_CONPOOL_SZ                  (sizeof (caf_conpool_t))

typedef struct caf_conpool_s caf_conpool_t;
struct caf_conpool_s {
	int con_id;
	int con_num;
	int *con_fds;
	caf_conn_t *con_seed;
	lstdl_t *con_lst;
};

caf_conpool_t *caf_conpool_new (int id, int num, caf_conn_t *seed);
int caf_conpool_delete (caf_conpool_t *svc);
int caf_conpool_init (caf_conpool_t *svc);
int caf_conpool_connect (caf_conpool_t *con);
int caf_conpool_stop (caf_conpool_t *svc);
int caf_conpool_close (caf_conpool_t *svc);
int caf_conpool_finalize (caf_conpool_t *svc);
int caf_conpool_reopen (caf_conpool_t *svc);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_IO_NET_CONPOOL_H */
/* caf_io_net_conpool.h ends here */
