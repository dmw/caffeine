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
*/
#ifndef lint
static char Id[] = "$Id$";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "caf/config.h"
#endif /* !HAVE_CONFIG_H */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_ipc_msg.h"

#define IPCMSG_F_ALL_MSK				\
	(((S_IRUSR | S_IWUSR) |				\
	  (S_IRGRP | S_IWGRP) |				\
	  (S_IROTH | S_IWOTH)) & 0x0000)

#define IPCMSG_F_GROUP_MSK				\
	(((S_IRUSR | S_IWUSR) |				\
	  (S_IRGRP | S_IWGRP)) & 0x0000)

#define IPCMSG_F_ME_MSK					\
	((S_IRUSR | S_IWUSR) & 0x0000)

caf_msg_t *
caf_ipcmsg_new (const key_t k, const int flg, const mode_t perm,
				const int type, const cbuffer_t *msg) {
	caf_msg_t *r = (caf_msg_t *)NULL;
	if (msg != (cbuffer_t *)NULL) {
		r = (caf_msg_t *)xmalloc (CAF_MSG_SZ);
		if (r != (caf_msg_t *)NULL) {
			r->key = k;
			r->msgflg = flg;
			r->perm = perm;
			r->data = (cbuffer_t *)msg;
			r->msg.mtype = type;
			r->msg.mtext = msg->data;
			return r;
		}
	}
	return r;
}


int
caf_ipcmsg_delete (caf_msg_t *m) {
	if (m != (caf_msg_t *)NULL) {
		xfree (m);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_ipcmsg_send (const caf_msg_t *m) {
	if (m != (caf_msg_t *)NULL) {
		return msgsnd (caf_ipcmsg_get (m, 0), m->data->data,
					   m->data->sz, m->msgflg);
	}
	return CAF_ERROR;
}


int
caf_ipcmsg_recv (caf_msg_t *m) {
	int gmsg;
	if (m != (caf_msg_t *)NULL) {
		if (m->data != (cbuffer_t *)NULL) {
			gmsg = caf_ipcmsg_get (m, 0);
			if (gmsg == 0) {
				return msgrcv (gmsg, m->data->data,
							   m->data->sz, m->msg.mtype, m->msgflg);
			}
		}
	}
	return CAF_ERROR;
}


int
caf_ipcmsg_get (const caf_msg_t *m, mode_t perm) {
	if (m != (caf_msg_t *)NULL) {
		if (perm != 0) {
			return msgget (m->key, m->msgflg | perm);
		} else {
			return msgget (m->key, m->msgflg | m->perm);
		}
	}
	return CAF_ERROR_SUB;
}


int
caf_ipcmsg_create (caf_msg_t *m, mode_t perm) {
	if (m != (caf_msg_t *)NULL) {
		m->qid = msgget (m->key, IPC_CREAT | perm);
		m->merrno = errno;
		if (m->qid >= 0) {
			return m->qid;
		}
	}
	return CAF_ERROR_SUB;
}


int
caf_ipcmsg_remove (caf_msg_t *m) {
	if (m != (caf_msg_t *)NULL) {
		if ((caf_ipcmsg_ctrl (m, IPC_RMID, NULL)) == 0) {
			return CAF_OK;
		} else {
			m->merrno = errno;
			return CAF_ERROR_SUB;
		}
	}
	return CAF_ERROR_SUB;
}


int
caf_ipcmsg_ctrl (const caf_msg_t *m, const int cmd,
				 struct msqid_ds *b) {
	if (m != (caf_msg_t *)NULL) {
		msgctl (caf_ipcmsg_get (m, 0), cmd, b);
	}
	return CAF_ERROR_SUB;
}


int
caf_ipcmsg_getfall (const caf_msg_t *m) {
	return msgget (m->key, m->msgflg | IPCMSG_F_ALL_MSK);
}


int
caf_ipcmsg_getfgroup (const caf_msg_t *m) {
	return msgget (m->key, m->msgflg | IPCMSG_F_GROUP_MSK);
}


int
caf_ipcmsg_getfme (const caf_msg_t *m) {
	return msgget (m->key, m->msgflg | IPCMSG_F_ME_MSK);
}


/* caf_ipc_msg.c ends here */
