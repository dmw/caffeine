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
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_ipc_msg.h"


const key_t msg_key = 0x333777;
const int msg_flags = 0;
const mode_t msg_perm = S_IRUSR | S_IWUSR;

cbuffer_t *msg_buffer;
caf_msg_t *msg_msg;
int msg_id;


void msg_send (char *str);
void msg_receive (void);


int
main (int argc, char **argv) {
	msg_buffer = cbuf_create (128);
	if (msg_buffer != (cbuffer_t *)NULL) {
		msg_msg = caf_ipcmsg_new (msg_key, 0, msg_perm, 0,
								  msg_buffer);
		if (msg_msg != (caf_msg_t *)NULL) {
			printf ("%s: msg = [%p]\n", argv[0], (void *)msg_msg);
			msg_id = caf_ipcmsg_create (msg_msg, msg_perm);
			if (msg_id >= 0) {
				msg_send ("hello from IPC\n");
				msg_receive ();
				caf_ipcmsg_remove (msg_msg);
				printf ("%s: msg_id = [%d]\n", argv[0], msg_id);
			} else {
				printf ("%s: can't create msg queue\n", argv[0]);
				exit (EXIT_FAILURE);
			}
		} else {
			printf ("%s: can't allocate msg\n", argv[0]);
			exit (EXIT_FAILURE);
		}
		caf_ipcmsg_delete (msg_msg);
	}
	return 0;
}


void msg_send (char *str) {
	if (msg_id >= 0 && msg_msg != (caf_msg_t *)NULL
		&& msg_buffer != (cbuffer_t *)NULL) {
		cbuf_clean (msg_buffer);
		memcpy (msg_buffer->data, (void *)str, strlen (str));
		if ((caf_ipcmsg_send (msg_msg)) == 0) {
			printf ("caf_ipcmsg_send: OK\n");
		}
	}
}


void msg_receive (void) {
	cbuffer_t *msg_buf;
	caf_msg_t *msg_recv;
	int recvsz;
	msg_buf = cbuf_create (128);
	if (msg_buf != (cbuffer_t *)NULL) {
		cbuf_clean (msg_buf);
		msg_recv = caf_ipcmsg_new (msg_key, msg_flags, msg_perm,
								   0, msg_buf);
		printf ("msg_recv: [%p]\n", (void *)msg_recv);
		if (msg_recv != (caf_msg_t *)NULL) {
			if ((recvsz = caf_ipcmsg_recv (msg_recv)) > 0) {
				printf ("received: OK\n\t%s", (char *)msg_buf->data);
			} else {
				printf ("received: ERROR\n");
			}
			printf ("recvsz: %d\n", recvsz);
			caf_ipcmsg_delete (msg_recv);
		}
		cbuf_delete (msg_buf);
	}
}


/* caf_ipcmsg.c ends here */
