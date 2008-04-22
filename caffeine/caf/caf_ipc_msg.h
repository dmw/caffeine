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
#ifndef CAF_IPC_MSG_H
#define CAF_IPC_MSG_H 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <caf/caf_data_buffer.h>

/**
 * @defgroup      caf_ipc_msg        IPC Messages
 * @ingroup       caf_ipc
 * @addtogroup    caf_ipc_msg
 * @{
 *
 * @brief     IPC Messages
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * IPC Messages
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define CAF_MSG_SZ              (sizeof (caf_msg_t))
#define CAF_MSG_CORE_SZ         (sizeof (caf_msg_core_t))

typedef struct caf_msg_core_s caf_msg_core_t;
struct caf_msg_core_s {
	long mtype;
	char *mtext;
};

typedef struct caf_msg_s caf_msg_t;
struct caf_msg_s {
	key_t key;
	int msgflg;
	mode_t perm;
	cbuffer_t *data;
	caf_msg_core_t msg;
};

caf_msg_t *caf_ipcmsg_new (const key_t k, const int flg, const mode_t perm,
						   const int type, const cbuffer_t *msg);
int caf_ipcmsg_delete (caf_msg_t *m);
int caf_ipcmsg_send (const caf_msg_t *m);
int caf_ipcmsg_recv (caf_msg_t *m);
int caf_ipcmsg_get (const caf_msg_t *m, const mode_t perm);
int caf_ipcmsg_create (const caf_msg_t *m, mode_t perm);
int caf_ipcmsg_remove (const caf_msg_t *m);
int caf_ipcmsg_ctrl (const caf_msg_t *m, const int cmd, struct msqid_ds *b);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_IPC_MSG_H */
/* caf_ipc_msg.h ends here */

