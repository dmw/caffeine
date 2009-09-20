/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "caf" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
/*
  Caffeine - C Application Framework
  Copyright (C) 2009 Daniel Molina Wegener <dmw@coder.cl>

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

#include <unistd.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_ipc_msg.h"
#include "caf/caf_ipc_msg_proto.h"
#include "caf/caf_psm.h"
#include "caf/caf_ssm.h"
#include "caf/caf_dsm.h"


#define CAF_MSG_SVC_SRCH_CB				caf_msg_svc_session_compare_id

static int caf_msg_svc_session_delete_cb(void *s);
static int caf_msg_svc_session_compare_id(void *s, void *id);


caf_msg_svc_t *
caf_ipcmsg_svc_create (caf_msg_t *seed,
					   caf_msg_svc_sm_t type,
					   void *machine) {
	caf_msg_svc_t *r = (caf_msg_svc_t *)NULL;
	if (seed == (caf_msg_t *)NULL) {
		return r;
	}
	r = (caf_msg_svc_t *)malloc (CAF_MSG_SVC_SZ);
	if (r == (caf_msg_svc_t *)NULL) {
		return r;
	}
	r->seed = seed;
	r->session_inc = 0;
	r->snd_inc = 0;
	r->rcv_inc = 0;
	r->sessions = lstdl_create ();
	if (r->sessions == (lstdl_t *)NULL) {
		xfree (r);
		r = (caf_msg_svc_t *)NULL;
	}
	if (type != MSG_SVC_MACHINE_NONE
		&& machine != NULL) {
		r->machine = machine;
		r->type = type;
	} else {
		r->machine = NULL;
		r->type = MSG_SVC_MACHINE_NONE;
	}
	return r;
}


int
caf_ipcmsg_svc_release (caf_msg_svc_t *s) {
	int rdel;
	if (s == (caf_msg_svc_t *)NULL) {
		return CAF_ERROR;
	}
	if (s->sessions != (lstdl_t *)NULL) {
		rdel = lstdl_delete (s->sessions, caf_msg_svc_session_delete_cb);
		if (rdel == CAF_OK) {
			s->sessions = (lstdl_t *)NULL;
		} else {
			return CAF_ERROR;
		}
	}
	xfree (s);
	return CAF_OK;
}


caf_msg_session_t *
caf_ipcmsg_svc_addses (caf_msg_svc_t *s) {
	caf_msg_session_t *r;
	if (s == (caf_msg_svc_t *)NULL) {
		return (caf_msg_session_t *)NULL;
	}
	r = (caf_msg_session_t *)malloc (CAF_MSG_SVC_SZ);
	s->session_inc++;
	s->snd_inc++;
	s->rcv_inc++;
	r->session_id = s->session_inc;
	r->snd_id = s->snd_inc;
	r->rcv_id = s->rcv_inc;
	r->snd_cnt = 0;
	r->rcv_cnt = 0;
	r->client = (pid_t)0;
	r->server = getpid();
	lstdl_push(s->sessions, (void *)r);
	return r;
}


int
caf_ipcmsg_svc_removeses (caf_msg_svc_t *s, long sid) {
	int r = CAF_ERROR;
	caf_msg_session_t *f;
	if (s == (caf_msg_svc_t *)NULL) {
		return r;
	}
	f = lstdl_search (s->sessions, &sid, CAF_MSG_SVC_SRCH_CB);
	if (f == (caf_msg_session_t *)NULL) {
		return r;
	}
	r = lstdl_node_delete_by_data (s->sessions, (void *)f,
								   caf_msg_svc_session_delete_cb);
	return r;
}


int
caf_ipcmsg_svc_process (caf_msg_svc_t *s,
						CAF_MSG_SVC_PCB(pcb)) {
	int cnt = CAF_ERROR;
	if (s == (caf_msg_svc_t *)NULL
		|| pcb == NULL) {
		s->errno_v = EINVAL;
		return cnt;
	}
	if (s->sessions == (lstdl_t *)NULL) {
		s->errno_v = EINVAL;
		return cnt;
	}
	switch (s->type) {
	case MSG_SVC_MACHINE_NONE:
		return lstdl_map (s->sessions, pcb);
	case MSG_SVC_MACHINE_STATIC:
		return lstdl_map (s->sessions, caf_msg_svc_run_ssm);
	case MSG_SVC_MACHINE_PLUGABLE:
		return lstdl_map (s->sessions, caf_msg_svc_run_psm);
	case MSG_SVC_MACHINE_DYNAMIC:
		return lstdl_map (s->sessions, caf_msg_svc_run_dsm);
	default:
		return lstdl_map (s->sessions, pcb);
		break;
	}
}


int
caf_msg_svc_run_ssm (void *data) {
	caf_msg_session_t *ses = (caf_msg_session_t *)data;
	caf_msg_svc_t *svc;
	if (data == NULL) {
		return CAF_ERROR;
	}
	svc = (caf_msg_svc_t *)ses->svc;
	if (svc == (caf_msg_svc_t *)NULL) {
		return CAF_ERROR;
	}
	return caf_ssm_runner_work ((caf_ssm_runner_t *)svc->machine, ses);
}


int
caf_msg_svc_run_psm (void *data) {
	caf_msg_session_t *ses = (caf_msg_session_t *)data;
	caf_msg_svc_t *svc;
	if (data == NULL) {
		return CAF_ERROR;
	}
	svc = (caf_msg_svc_t *)ses->svc;
	if (svc == (caf_msg_svc_t *)NULL) {
		return CAF_ERROR;
	}
	return caf_psm_runner_work ((caf_ssm_runner_t *)svc->machine, ses);
}


int
caf_msg_svc_run_dsm (void *data) {
	caf_msg_session_t *ses = (caf_msg_session_t *)data;
	caf_msg_svc_t *svc;
	if (data == NULL) {
		return CAF_ERROR;
	}
	svc = (caf_msg_svc_t *)ses->svc;
	if (svc == (caf_msg_svc_t *)NULL) {
		return CAF_ERROR;
	}
	return caf_dsm_runner_work ((caf_ssm_runner_t *)svc->machine, ses);
}


static int
caf_msg_svc_session_delete_cb(void *s) {
	if (s == NULL) {
		return CAF_ERROR;
	}
	xfree(s);
	return CAF_OK;
}


static int
caf_msg_svc_session_compare_id(void *s, void *id) {
	caf_msg_session_t *cs = (caf_msg_session_t *)s;
	u_long *cid = (u_long *)id;
	if (s == NULL || id == NULL) {
		return CAF_ERROR;
	}
	return (cs->session_id == *cid) ? CAF_OK : CAF_ERROR;
}

/* caf_ipc_msg_proto.c ends here */
