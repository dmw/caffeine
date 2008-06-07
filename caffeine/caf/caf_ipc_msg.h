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
 * @author    Daniel Molina Wegener <dmw@coder.cl>
 *
 * IPC Messages
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Size of the <b>caf_msg_t</b> structure */
#define CAF_MSG_SZ              (sizeof (caf_msg_t))
/** Size of the <b>caf_msg_core_t</b> structure */
#define CAF_MSG_CORE_SZ         (sizeof (caf_msg_core_t))

/**
 * @brief Message Core Structure Type
 *
 * C typedef for the <b>caf_msg_core_s</b> structure.
 *
 * @see caf_msg_core_s
 */
typedef struct caf_msg_core_s caf_msg_core_t;

/**
 * @brief Message Core Structure
 *
 * Basic structure holding information about IPC messages.
 * Consist on message type and buffer oriented (char *)
 * message content.
 *
 * @see caf_msg_core_s
 */
struct caf_msg_core_s {
	/** Message Type, user definition of message type */
	long mtype;
	/** Message Text, user pointer to message data */
	char *mtext;
};

/**
 * @brief Caffeine IPC Message Definition Type
 *
 * C typedef for the <b>caf_msg_s</b> structure.
 *
 * @see caf_msg_s
 */
typedef struct caf_msg_s caf_msg_t;

/**
 * @brief Caffeine IPC Message Structure
 *
 * Holds basic information about messages, the IPC key <b>key</b>,
 * IPC messaging flags <b>msgflg</b>, key permissions <b>perm</b>,
 * buffer oriented message content <b>data</b> and core definition
 * of messages according to the <b>msgsnd(2)</b> and <b>msgrcv(2)</b>
 * needs of <b>msgbuf</b> structure.
 *
 * @see caf_msg_t
 * @see cbuffer_t
 */
struct caf_msg_s {
	/** IPC key */
	key_t key;
	/** Messaging Flags */
	int msgflg;
	/** Message Queue Identifier */
	int qid;
	/** Key permissions */
	mode_t perm;
	/** Buffer data */
	cbuffer_t *data;
	/** Core message accoring to <b>msgbuf</b> needs */
	caf_msg_core_t msg;
	/** Message <b>errno(3)</b> trap */
	int merrno;
};

/** 
 * @brief Allocates memory for a <b>caf_msg_t</b> structure
 *
 * Allocates enough memory for a <b>caf_msg_t</b> structure
 * pointer, setting default values to the propper parameters
 * to start working with messages.
 * 
 * @param k[in]		ipc key
 * @param flg[in]	messaging flags
 * @param perm[in]  key permissions
 * @param type[in]  user message definiton
 * @param msg[in]	user allocated message through <b>cbuffer_t</b>
 * 
 * @return caf_msg_t	a new and clear allocated pointer.
 */
caf_msg_t *caf_ipcmsg_new (const key_t k, const int flg, const mode_t perm,
						   const int type, const cbuffer_t *msg);

/** 
 * @brief Deallocates a <b>caf_msg_t</b> pointer
 *
 * Deallocates the given <b>caf_msg_t</b> pointer, but does not
 * closes and removes the <i>ipc key</i> contained in the given
 * structure, it just releases the memory block through <b>free(3)</b>.
 * 
 * @param m[in]			pointer to deallocate.
 * 
 * @return int	CAF_OK on success, CAF_ERROR on failure
 */
int caf_ipcmsg_delete (caf_msg_t *m);

/** 
 * @brief Sends messages through <b>msgsnd(2)</b>
 *
 * Sends the given <b>@link #caf_msg_t @endlink</b> pointer through
 * <b>msgsnd(2)</b> system call. Do not send the complete structure,
 * instead uses the <b>@link #cbuffer_t @endlink</b> structure memeber
 * <b>@link #cbuffer_t#data @endlink</b> to send the message, according
 * to the <b>I/O</b> information that <b>@link #cbuffer_t @endlink</b>
 * holds, using the <b>@link #cbuffer_t#sz @endlink</b> member as
 * <i>message size</i> parameter.
 *
 * @param m[in]			message to send.
 * 
 * @return int	CAF_OK (zero) on success, distinct to zero on failure.
 */
int caf_ipcmsg_send (const caf_msg_t *m);

/** 
 * @brief Receives IPC messages
 * 
 * <p>Receives IPC messages through the <b>msgrcv(2)</b> system call.
 * Also, uses the <b>@link #caf_msg_t @endlink</b> structure to
 * pass parameters to the system call. Also, calls <b>msgget(2)</b>
 * system call through <b>@link caf_ipcmsg_get() @endlink</b> to
 * retreive the next available <i>message identifier</i>.</p>
 *
 * <p>Uses the <b>@link #caf_msg_t#data @endlink</b> with the
 * <b>@link #cbuffer_t#data @link</b> as message data structure.
 * Sets the IPC <i>message type</i> for the <b>msgrcv(2)</b>
 * using the <b>@link #caf_msg_t#msg @endlink</b> as message
 * definition, taking the member <b>@link #caf_msg_core_t#mtype
 * @endlink</b> as <i>message type</i> parameter. The <i>message
 * flags</i> are passed to <b>msgrcv(2)</b> as <b>@link #caf_msg_t#msgflg
 * @endlink</b>, accorind with <b>msgrcv(2)</b> interface.</p>
 *
 * @param m[in]			structure to hold the received message.
 * 
 * @return int	CAF_OK (zero) on success, non zero on failure.
 */
int caf_ipcmsg_recv (caf_msg_t *m);

/** 
 * @brief Retreives message identifiers
 *
 * <p>Retreives message identifiers through <b>msgget(2)</b> system
 * call. Uses <b>@link #caf_msg_t#data @endlink</b> member of type
 * <b> @link #cbuffer_t#data @endlink</b> as <i>data</i> and
 * <i>size</i> and the propper previosly setted types and flags.</p>
 *
 * <p>The <b>mode_t</b> parameter <b>perm</b> sets a diferent
 * permissions on <b>msgget(2)</b> system call, instead of the
 * permissions defined in the <b>@link #caf_msg_t#perm @endlink</b>
 * structure memeber.</p>
 *
 * <p><i>Note that if you use this interface, you must retreive the
 * messages by your own. This interface is defined for <b>caffeine</b>
 * internal use.</i></p>
 * 
 * @param m[in]			message to get
 * @param perm[in]		permissions if you want to use different ones.
 * 
 * @return int	CAF_OK (zero) on success, distinct to zero on failure.
 */
int caf_ipcmsg_get (const caf_msg_t *m, const mode_t perm);

/** 
 * @brief Creates a message queue
 *
 * <p>Creates a <i>message queue</i> using the <b>msgget(2)</b> system
 * call. The <b>key</b> parameter used on the call it's equivalent to
 * the <b>@link #caf_msg_t#key @endlink</p> member, then you must set
 * the right key argument when you are calling the
 * <b>@link caf_ipcmsg_new() @endlink</p> caffeine interface.</p>
 *
 * <p>Also, uses the <b>IPC_CREAT</b> flag and sets the permissions
 * passed as <b>perm</b> parameter. Remember to use the permission
 * symbols defined in <b>chmod(2)</b> as <i>POSIX</i> requires, not
 * the permissions using <i>numeric constants</i>. You can use the
 * <b>@link #caf_msg_t#errno @endling</b> trap to know which error
 * was triggered with the system call failure.</p>
 * 
 * @param m[in]			the message structure to use as base.
 * @param perm[in]		queue permissions
 * 
 * @return int			queue identifier
 */
int caf_ipcmsg_create (caf_msg_t *m, mode_t perm);

/** 
 * @brief Removes a message queue
 *
 * <p>Removes an <i>IPC Message Queue</i> by using the
 * <b>@link caf_ipcmsg_ctrl() @endlink</b>, and inside
 * the <b>msgctl(2)</b> system call.</p>
 *
 * <p>It uses the data stored in the <b>@link #caf_msg_t @endlink</b>
 * structure to pass parameters through the calls.</p>
 * 
 * @param m[in]			message definition to use.
 * 
 * @return int	CAF_OK (zero) on success, CAF_ERROR_SUB on failure
 */
int caf_ipcmsg_remove (caf_msg_t *m);

/** 
 * @brief Controls message queues
 *
 * <p>Applies <b>msgctl(2)</b> system call to the given
 * <i>Message Queue</i> structure, passing the given
 * arguments to the system call. Also stores the errno value to the
 * <b>@link #caf_msg_t#errno @endlink</b> member, retaining information
 * about the call.</p>
 *
 * <p>The command argument <b>cmd</b> it's equivalent to the
 * <i>command</i> argument for the <b>msgctl(2)</b> system call. And
 * the <b>msqid_ds</b> argument it's equivalent to the argument
 * passed to the <b>msqid_ds</b> argument passed to the <b>msgctl(2)</b>
 * system call. You must try to use common members to the <i>POSIX</i>
 * specification instead of <i>operating system specific</i> members.</p>
 *
 * @param m[in]			message queue to use
 * @param cmd[in]		command to apply
 * @param b				information in/out buffer
 * 
 * @return int	CAF_ERROR_SUB on failure, on success the system
 *				call returned value
 */
int caf_ipcmsg_ctrl (const caf_msg_t *m, const int cmd, struct msqid_ds *b);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_IPC_MSG_H */
/* caf_ipc_msg.h ends here */

