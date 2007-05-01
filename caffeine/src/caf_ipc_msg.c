/* -*- mode: c; c-default-style: "k&r"; indent-tabs-mode: nil; -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 expandtab: */
/* kate: space-indent on; indent-width 4; mixedindent off; indent-mode none; */
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
#ifndef lint
static char Id[] = "$Id: caf_ipc_msg.c 183 2007-02-01 20:07:24Z dmw $";
#endif /* !lint */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_ipc_msg.h"


caf_msg_t *
caf_ipcmsg_new (const key_t k, const int flg, const int type,
                const cbuffer_t *msg)
{
    caf_msg_t *r = (caf_msg_t *)NULL;
    if (msg != (cbuffer_t *)NULL && flg > 0 && type > 0) {
        r = (caf_msg_t *)xmalloc (CAF_MSG_SZ);
        if (r != (caf_msg_t *)NULL) {
            r->key = k;
            r->msgflg = flg;
            r->data = (cbuffer_t *)msg;
            r->msg.mtype = type;
            r->msg.mtext = msg->data;
            return r;
        }
    }
    return r;
}


int
caf_ipcmsg_delete (caf_msg_t *m)
{
    if (m != (caf_msg_t *)NULL) {
        cbuf_delete (m->data);
        xfree (m);
        return CAF_OK;
    }
    return CAF_ERROR;
}


int
caf_ipcmsg_send (const caf_msg_t *m)
{
    if (m != (caf_msg_t *)NULL) {
        return msgsnd (caf_ipcmsg_get (m), m->data->data, m->data->sz,
                       m->msgflg);
    }
    return CAF_ERROR;
}


int
caf_ipcmsg_recv (caf_msg_t *m)
{
    if (m != (caf_msg_t *)NULL) {
        if (m->data != (cbuffer_t *)NULL) {
            return msgrcv (caf_ipcmsg_get (m), m->data->data, m->data->sz,
                           m->msg.mtype, m->msgflg);
        }
    }
    return CAF_ERROR;
}


int
caf_ipcmsg_get (const caf_msg_t *m)
{
    if (m != (caf_msg_t *)NULL) {
        return msgget (m->key, m->msgflg);
    }
    return CAF_ERROR_SUB;
}


int
caf_ipcmsg_ctrl (const caf_msg_t *m, const int cmd, struct msqid_ds *b)
{
    if (m != (caf_msg_t *)NULL) {
        return msgctl (caf_ipcmsg_get (m), cmd, b);
    }
    return CAF_ERROR;
}

/* caf_ipc_msg.c ends here */
