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

  $Id$
*/
#ifndef CAF_IPC_MSG_PROTO_H
#define CAF_IPC_MSG_PROTO_H 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <caf/caf_data_buffer.h>
#include <caf/caf_ipc_msg.h>
#include <caf/caf_psm.h>
#include <caf/caf_ssm.h>
#include <caf/caf_dsm.h>
#include <caf/caf_data_packer.h>

/**
 * @defgroup      caf_ipc_msg_proto     IPC Messages Lightweight Protocol
 * @ingroup       caf_ipc
 * @addtogroup    caf_ipc_msg_proto
 * @{
 *
 * @brief     IPC Messages Lightweight Protocol
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@coder.cl>
 *
 * A lightweight protocol as client-server tratment to allow
 * you to friendly process SysV IPC messaging API. This can
 * allow you to setup single messages from and to client and
 * server, transparent communications and due to the SysV IPC
 * messaging API restrictions, it is acting as an extension
 * to the SysV IPC messaging API.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define CAF_MSG_SESSION_SZ				(sizeof(caf_msg_session_t))
#define CAF_MSG_SVC_SZ					(sizeof(caf_msg_svc_t))
#define CAF_MSG_SVC_PCB(pcb)			int (*pcb)(void *s)
#define CAF_MSG_SVC_LOCKED				1
#define CAF_MSG_SVC_UNLOCKED			0

/**
 * @brief Messaging Session Type
 *
 * C typedef for the <b>@link #caf_msg_session_s @endlink</b>
 * structure.
 *
 * @see caf_msg_session_s
 */
typedef struct caf_msg_session_s caf_msg_session_t;

/**
 * @brief Message Server Type
 *
 * C typedef for the <b>@link #caf_msg_svc_s @endlink</b>
 * structure.
 *
 * @see caf_msg_svc_s
 */
typedef struct caf_msg_svc_s caf_msg_svc_t;

/**
 * @brief Messaging Session Structure
 *
 * The Messaging Session Structure allows you to setup a
 * client and server session between known two process,
 * one acts as server and other acts as client. The
 * server endpoint find messages by using the data stored
 * about his clients in <b>@link #caf_msg_svc_t @endling</b>.
 * Also, the client must have a copy of the session to
 * send the proper messages to his server.
 *
 */
struct caf_msg_session_s {
	/** Session identifier */
	u_long session_id;
	/** Sender identifier */
	u_long snd_id;
	/** Receiver identifier */
	u_long rcv_id;
	/** Messages sent counter */
	u_long snd_cnt;
	/** Messages received counter */
	u_long rcv_cnt;
	/** Session message size */
	size_t sz;
	/** Client PID */
	pid_t client;
	/** Server PID */
	pid_t server;
	/** Lock */
	int locked;
	/** Service holder */
	caf_msg_svc_t *svc;
};


/**
 * @brief Message Service State Machine Proccessing Types
 *
 * State Machine selector to process the Messaging Service
 *
 * @see caf_psm
 * @see caf_dsm
 * @see caf_ssm
 */
typedef enum {
	/** The state machine has no type and is not used */
	MSG_SVC_MACHINE_NONE = 0,
	/** The state machine is of type @see caf_ssm */
	MSG_SVC_MACHINE_STATIC = 100,
	/** The state machine is of type @see caf_psm */
	MSG_SVC_MACHINE_PLUGABLE = 200,
	/** The state machine is of type @see caf_dsm */
	MSG_SVC_MACHINE_DYNAMIC = 300
} caf_msg_svc_sm_t;


/**
 * @brief Message Server Structure
 *
 * A Message Server Structure holds the proper data about serving
 * queues and his clients. Usually a master proccess which is
 * sending/receiving data from its clients. This is designed
 * to send and receive small amounts of data, known as
 * <b>commands</b>. It holds its sessions in a <b>deque</b>
 * called <b>sessions</b> of the type
 * @link #caf_msg_session_t @endlink.
 *
 */
struct caf_msg_svc_s {
	/** Seed and sample message */
	caf_msg_t *seed;
	/** Session identifier increment */
	u_long session_inc;
	/** Sender identifier increment */
	u_long snd_inc;
	/** Receiver identifier increment */
	u_long rcv_inc;
	/** List of sessions */
	lstdl_t *sessions;
	/** Processing Machine */
	void *machine;
	/** Processing Machine Type */
	caf_msg_svc_sm_t type;
	/** The Packet parser using Data Packer @see caf_data_packer */
	caf_packet_t *parser;
	/** errno value for common operations */
	int errno_v;
};


caf_msg_svc_t *caf_ipcmsg_svc_create (caf_msg_t *seed,
									  caf_msg_svc_sm_t type,
									  void *machine,
									  caf_packet_t *parser);

int caf_ipcmsg_svc_release (caf_msg_svc_t *s);

caf_msg_session_t *caf_ipcmsg_svc_addses (caf_msg_svc_t *s);

int caf_ipcmsg_svc_removeses (caf_msg_svc_t *s, long sid);

static int caf_msg_svc_session_delete_cb(void *s);

static int caf_msg_svc_session_compare_id(void *s, void *id);

int caf_ipcmsg_svc_process (caf_msg_svc_t *s, CAF_MSG_SVC_PCB(pcb));

int caf_msg_svc_run_ssm (void *data);

int caf_msg_svc_run_psm (void *data);

int caf_msg_svc_run_dsm (void *data);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_IPC_MSG_PROTO_H */
/* caf_ipc_msg.h ends here */
