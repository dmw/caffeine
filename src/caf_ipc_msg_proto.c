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
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>

#include <unistd.h>

#include "caf/caf.h"
#include "caf/caf_mem.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_data_deque.h"
#include "caf/caf_ipc_msg.h"
#include "caf/caf_ipc_msg_proto.h"
#include "caf/caf_psm.h"
#include "caf/caf_ssm.h"
#include "caf/caf_dsm.h"


#define CAF_MSG_SVC_SRCH_CB				caf_msg_svc_session_compare_id

static int caf_msg_svc_session_delete_cb(void *s);
static int caf_msg_svc_session_compare_id(void *s, void *id);

void *std_sm_packet_process (void *s_data, caf_dsm_return_t *s_return);

caf_dsm_return_t *dsm_packet_process(void *s_data,
									 caf_dsm_return_t *s_return);

caf_psm_return_t *psm_packet_process(void *s_data,
									 caf_psm_return_t *s_return);

caf_ssm_return_t *ssm_packet_process(void *s_data,
									 caf_ssm_return_t *s_return);


caf_msg_svc_t *
caf_ipcmsg_svc_create (caf_msg_t *seed,
					   caf_msg_svc_sm_t type,
					   void *machine,
					   caf_packet_t *parser) {
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
	r->sessions = deque_create ();
	r->parser = (caf_packet_t *)NULL;
	if (r->sessions == (deque_t *)NULL) {
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
	if (parser != (caf_packet_t *)NULL) {
		r->parser = parser;
	}
	return r;
}


int
caf_ipcmsg_svc_release (caf_msg_svc_t *s) {
	int rdel;
	if (s == (caf_msg_svc_t *)NULL) {
		return CAF_ERROR;
	}
	if (s->sessions != (deque_t *)NULL) {
		rdel = deque_delete (s->sessions, caf_msg_svc_session_delete_cb);
		if (rdel == CAF_OK) {
			s->sessions = (deque_t *)NULL;
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
	deque_push(s->sessions, (void *)r);
	return r;
}


int
caf_ipcmsg_svc_removeses (caf_msg_svc_t *s, long sid) {
	int r = CAF_ERROR;
	caf_msg_session_t *f;
	if (s == (caf_msg_svc_t *)NULL) {
		return r;
	}
	f = deque_search (s->sessions, &sid, CAF_MSG_SVC_SRCH_CB);
	if (f == (caf_msg_session_t *)NULL) {
		return r;
	}
	r = deque_node_delete_by_data (s->sessions, (void *)f,
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
	if (s->sessions == (deque_t *)NULL) {
		s->errno_v = EINVAL;
		return cnt;
	}
	switch (s->type) {
	case MSG_SVC_MACHINE_NONE:
		return deque_map (s->sessions, pcb);
	case MSG_SVC_MACHINE_STATIC:
		return deque_map (s->sessions, caf_msg_svc_run_ssm);
	case MSG_SVC_MACHINE_PLUGABLE:
		return deque_map (s->sessions, caf_msg_svc_run_psm);
	case MSG_SVC_MACHINE_DYNAMIC:
		return deque_map (s->sessions, caf_msg_svc_run_dsm);
	default:
		return deque_map (s->sessions, pcb);
		break;
	}
}


int
caf_ipcmsg_svc_convert_pack (caf_msg_svc_t *s) {
	caf_dsm_t *dsmt;
	caf_dsm_state_t *dsm;
	caf_dsm_runner_t *dsm_r;
	caf_psm_t *psmt;
	caf_psm_state_t *psm;
	caf_psm_runner_t *psm_r;
	size_t fsz, lsz;
	if (s == (caf_msg_svc_t *)NULL) {
		return CAF_ERROR;
	}
	if (s->parser == (caf_packet_t *)NULL) {
		s->errno_v = EINVAL;
		return CAF_ERROR;
	}
	if (s->machine == NULL) {
		s->errno_v = EINVAL;
		return CAF_ERROR;
	}
	switch (s->type) {
	case MSG_SVC_MACHINE_STATIC:
#if 0
		ssm = caf_ssm_state_new (0, CAF_PSM_STATE_START,
								 ssm_packet_process,
								 NULL);
		ssm_r = (caf_ssm_runner_t *)s->machine;
		if (s->machine == (caf_ssm_runner_t *)NULL
			|| ((caf_ssm_runner_t *)s->machine)->r_machine == NULL) {
			caf_ssm_state_delete(ssm);
			s->errno_v = EINVAL;
			return CAF_ERROR;
		}
		ssmt = ((caf_ssm_t *)((caf_ssm_runner_t *)s->machine)->r_machine);
		lsz = ((size_t)(ssmt->last)) * (sizeof(caf_ssm_t));
		fsz = lsz + (sizeof(caf_ssm_t));
		ssmt->m_state = (caf_ssm_state_t **)realloc (ssmt->m_state, fsz);
		xmemcpy((void *)((size_t)ssmt->m_state
						 + sizeof(caf_ssm_t)), ssmt->m_state, lsz);
		ssmt->m_state[0] = ssm;
		ssmt->last++;
		return CAF_OK;
#endif /* SSM is not allowed to change */
		s->errno_v = EINVAL;
		return CAF_ERROR;
	case MSG_SVC_MACHINE_PLUGABLE:
		psm = caf_psm_state_new (0, CAF_PSM_STATE_START,
								 psm_packet_process,
								 NULL);
		psm_r = (caf_psm_runner_t *)s->machine;
		if (s->machine == (caf_psm_runner_t *)NULL
			|| ((caf_psm_runner_t *)s->machine)->r_machine == NULL) {
			caf_psm_state_delete(psm);
			s->errno_v = EINVAL;
			return CAF_ERROR;
		}
		psmt = ((caf_psm_t *)((caf_psm_runner_t *)s->machine)->r_machine);
		lsz = ((size_t)(psmt->last)) * (sizeof(caf_psm_t));
		fsz = lsz + (sizeof(caf_psm_t));
		psmt->m_state = (caf_psm_state_t *)realloc (psmt->m_state, fsz);
		xmemcpy((void *)((size_t)psmt->m_state
						 + sizeof(caf_psm_t)), psmt->m_state, lsz);
		psmt->m_state[0] = *psm;
		psmt->last++;
		return CAF_OK;
	case MSG_SVC_MACHINE_DYNAMIC:
		dsm = caf_dsm_state_new (0, CAF_DSM_STATE_START,
								 dsm_packet_process,
								 NULL);
		dsm_r = (caf_dsm_runner_t *)s->machine;
		if (s->machine == (caf_dsm_runner_t *)NULL
			|| ((caf_dsm_runner_t *)s->machine)->r_machine == NULL) {
			caf_dsm_state_delete(dsm);
			s->errno_v = EINVAL;
			return CAF_ERROR;
		}
		dsmt = ((caf_dsm_t *)((caf_dsm_runner_t *)s->machine)->r_machine);
		deque_insert(dsmt->m_state, 0, dsm);
		return CAF_OK;
	default:
		s->errno_v = EINVAL;
		return CAF_ERROR;
	}
}


void *
std_sm_packet_process (void *s_data, caf_dsm_return_t *s_return) {
	caf_msg_session_t *s;
	caf_dsm_return_t *r;
	caf_msg_t *seed;
	caf_msg_t *nmsg;
	cbuffer_t *buf;
	if (s_data == NULL) {
		return NULL;
	}
	if (s_return == NULL) {
		s = (caf_msg_session_t *)s_data;
		if (s->svc == (caf_msg_svc_t *)NULL) {
			return NULL;
		}
		if (s->msg == NULL) {
			seed = s->svc->seed;
			buf = cbuf_create (caf_packet_getsize(s->svc->parser));
			cbuf_clean (buf);
			nmsg = caf_ipcmsg_new ((key_t)seed->key, seed->msgflg, seed->perm,
								   seed->msg.mtype, buf);
			s->msg = nmsg;
		} else {
			nmsg = s->msg;
			cbuf_clean (s->msg->data);
		}
		if ((caf_ipcmsg_recv (s->msg)) == CAF_ERROR) {
			return NULL;
		}
		if ((caf_packet_parse_machine (s->svc->parser,
									   (cbuffer_t *)s->msg->data))
			== CAF_ERROR) {
			return NULL;
		}
		r = (caf_dsm_return_t *)xmalloc (sizeof(caf_dsm_return_t));
		if (r == NULL) {
			return NULL;
		}
		r->r_control = CAF_DSM_CONTROL_FORWARD;
		r->r_data = s_data;
		r->r_return = s->svc->parser;
		return (void *)r;
	}
	return NULL;
}


caf_dsm_return_t *dsm_packet_process (void *s_data,
									  caf_dsm_return_t *s_return) {
	caf_dsm_return_t *r;
	r = (caf_dsm_return_t *)std_sm_packet_process (s_data, s_return);
	return r;
}


caf_psm_return_t *psm_packet_process (void *s_data,
									  caf_psm_return_t *s_return) {
	caf_psm_return_t *r;
	r = (caf_psm_return_t *)std_sm_packet_process (s_data,
												   (caf_dsm_return_t *)s_return);
	return r;
}


caf_ssm_return_t *ssm_packet_process (void *s_data,
									  caf_ssm_return_t *s_return) {
	caf_ssm_return_t *r;
	r = (caf_ssm_return_t *)std_sm_packet_process (s_data,
												   (caf_dsm_return_t *)s_return);
	return r;
}


int
caf_ipcmsg_svc_process_packer (caf_msg_svc_t *s,
							   CAF_MSG_SVC_PCB(pcb)) {
	int cnt = CAF_ERROR;
	if (s == (caf_msg_svc_t *)NULL
		|| pcb == NULL) {
		return cnt;
	}
	if (s->sessions == (deque_t *)NULL) {
		s->errno_v = EINVAL;
		return cnt;
	}
	switch (s->type) {
	case MSG_SVC_MACHINE_NONE:
		return deque_map (s->sessions, pcb);
	case MSG_SVC_MACHINE_STATIC:
		return deque_map (s->sessions, caf_msg_svc_run_ssm);
	case MSG_SVC_MACHINE_PLUGABLE:
		return deque_map (s->sessions, caf_msg_svc_run_psm);
	case MSG_SVC_MACHINE_DYNAMIC:
		return deque_map (s->sessions, caf_msg_svc_run_dsm);
	default:
		return deque_map (s->sessions, pcb);
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
	return caf_ssm_runner_work ((caf_ssm_runner_t *)svc->machine,
								ses);
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
	return caf_psm_runner_work ((caf_psm_runner_t *)svc->machine,
								ses);
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
	return caf_dsm_runner_work ((caf_dsm_runner_t *)svc->machine,
								ses);
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
