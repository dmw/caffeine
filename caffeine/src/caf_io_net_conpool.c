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
static char Id[] = "$Id$";
#endif /* !lint */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <unistd.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_io_file.h"
#include "caf/caf_io_net.h"
#include "caf/caf_io_net_conpool.h"


static int caf_conpool_delete_callback (void *data);

caf_conpool_t *
caf_conpool_new (int id, int num, caf_conn_t *seed)
{
    caf_conpool_t *r = (caf_conpool_t *)NULL;
    if (id > 0 && num > 0 && seed != (caf_conn_t *)NULL) {
        r = (caf_conpool_t *)xmalloc (CAF_CONPOOL_SZ);
        if (r != (caf_conpool_t *)NULL) {
            r->con_num = num;
        }
    }
    return r;
}


int
caf_conpool_delete (caf_conpool_t *con)
{
    if (con != (caf_conpool_t *)NULL) {
        if ((lstdl_delete (con->con_lst, caf_conpool_delete_callback)) ==
            CAF_OK) {
            xfree (con->con_fds);
            xfree (con);
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


static int
caf_conpool_delete_callback (void *data)
{
    caf_conn_t *con;
    if (data != (void *)NULL) {
        con = (caf_conn_t *)data;
        return caf_conn_delete (con);
    }
    return CAF_ERROR;
}


int
caf_conpool_init (caf_conpool_t *con)
{
    int *fds, c;
    caf_conn_t *i, *sc;
    if (con != (caf_conpool_t *)NULL) {
        if (con->con_seed != (caf_conn_t *)NULL) {
            con->con_fds = (int *)xmalloc ((size_t)con->con_num * sizeof (int));
            con->con_lst = lstdl_create ();
            if (con->con_fds != (int *)NULL &&
                con->con_lst != (lstdl_t *)NULL) {
                memset (con->con_fds, (int)NULL,
                        (size_t)con->con_num * sizeof (int));
                fds = con->con_fds;
                sc = con->con_seed;
                for (c = 0; c < con->con_num; c++) {
                    fds[c] = -1;
                    fds[c] = dup (sc->sock);
                    if (fds[c] >= 0) {
                        i = caf_conn_new (fds[c], sc->flags, sc->addrlen,
                                          sc->saddr, sc->daddr);
                        if (i != (caf_conn_t *)NULL) {
                            i->flags = sc->flags;
                            i->dom = sc->dom;
                            i->type = sc->type;
                            lstdl_push (con->con_lst, (void *)i);
                        }
                    } else {
                        lstdl_push (con->con_lst, (void *)NULL);
                    }
                }
                return CAF_OK;
            }
        }
    }
    return CAF_ERROR;
}


int
caf_conpool_connect (caf_conpool_t *con)
{
    int r = 0;
    caf_conn_t *i;
    lstdln_t *n;
    if (con != (caf_conpool_t *)NULL) {
        if (con->con_seed != (caf_conn_t *)NULL) {
            con->con_fds = (int *)xmalloc ((size_t)con->con_num * sizeof (int));
            con->con_lst = lstdl_create ();
            if (con->con_fds != (int *)NULL &&
                con->con_lst != (lstdl_t *)NULL) {
                n = con->con_lst->frst;
                while (n != (lstdln_t *)NULL) {
                    i = (caf_conn_t *)n->data;
                    r += caf_conn_options_clone (i, con->con_seed);
                    r += caf_conn_connect (i);
                    n = n->next;
                }
                return CAF_OK;
            }
        }
    }
    return CAF_ERROR;
}


int
caf_conpool_stop (caf_conpool_t *con)
{
    int c, fd, r;
    if (con != (caf_conpool_t *)NULL) {
        if (con->con_fds != (int *)NULL && con->con_lst != (lstdl_t *)NULL) {
            for (c = 0; c <= con->con_num; c++) {
                fd = con->con_fds[c];
                if (fd > -1) {
                    r += shutdown (fd, SHUT_RDWR);
                }
            }
        }
    }
    return CAF_ERROR;
}


int
caf_conpool_close (caf_conpool_t *con)
{
    int c, fd, r;
    if (con != (caf_conpool_t *)NULL) {
        if (con->con_fds != (int *)NULL && con->con_lst != (lstdl_t *)NULL) {
            for (c = 0; c <= con->con_num; c++) {
                fd = con->con_fds[c];
                if (fd > -1) {
                    r += close (fd);
                }
            }
        }
    }
    return CAF_ERROR;
}


int
caf_conpool_finalize (caf_conpool_t *con)
{
    int c, fd, r;
    if (con != (caf_conpool_t *)NULL) {
        if (con->con_fds != (int *)NULL && con->con_lst != (lstdl_t *)NULL) {
            for (c = 0; c <= con->con_num; c++) {
                fd = con->con_fds[c];
                if (fd > -1) {
                    r += shutdown (fd, SHUT_RDWR);
                    r += close (fd);
                }
            }
        }
    }
    return CAF_ERROR;
}


int
caf_conpool_reopen (caf_conpool_t *con)
{
    int c = 0, r = 0;
    lstdln_t *n;
    caf_conn_t *cc, *seed, *rn;
    if (con != (caf_conpool_t *)NULL) {
        if (con->con_fds != (int *)NULL && con->con_lst != (lstdl_t *)NULL &&
            con->con_seed != (caf_conn_t *)NULL) {
            seed = con->con_seed;
            n = con->con_lst->frst;
            c = 0;
            while (n != (lstdln_t *)NULL) {
                cc = (caf_conn_t *)n->data;
                rn = caf_conn_new (seed->sock, seed->flags, seed->addrlen,
                                   seed->saddr, seed->daddr);
                if (rn != (caf_conn_t *)NULL) {
                    caf_conn_options_clone (rn, seed);
                    r += shutdown (cc->sock, SHUT_RDWR);
                    r += close (cc->sock);
                    if ((caf_conn_connect (rn)) == CAF_OK) {
                        n->data = (void *)rn;
                        caf_conn_delete (cc);
                    } else {
                        cc->sock = -1;
                    }
                }
                n = n->next;
                c++;
            }
        }
    }
    return r;
}

/* caf_io_net_conpool.c ends here */
