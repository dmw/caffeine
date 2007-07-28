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

    $Id$
 */
#ifndef CAF_IO_NET_H
#define CAF_IO_NET_H 1
/**
 * @defgroup      caf_io_net            Network I/O
 * @ingroup       caf_io
 * @addtogroup    caf_io_net
 * @{
 *
 * @brief     Network I/O
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Network I/O
 *
 */

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#define CAF_CONNECTION_SZ               (sizeof (caf_conn_t))

	typedef enum {
	    CAF_CONN_FREE_SRC = 0000001,
	    CAF_CONN_FREE_DST = 0000002,
	    CAF_CONN_INCOMING = 0000004,
	    CAF_CONN_OUTGOING = 0000010,
	    CAF_CONN_NONBLOCK = 0000020,
	    CAF_CONN_SOCKSEED = 0000040
	} caf_connection_flags_t;

	typedef enum {
	    CAF_CONN_NOLEVEL = 0000000,
	    CAF_CONN_SOCKOPTS = 0000001,
	    CAF_CONN_FCNTL = 0000002
	} caf_connection_mode_t;

	typedef struct caf_conn_s caf_conn_t;
	struct caf_conn_s {
		int sock;
		caf_connection_flags_t flags;
		int dom;
		int type;
		int proto;
		socklen_t addrlen;
		struct sockaddr *saddr;
		struct sockaddr *daddr;
	};

	caf_conn_t *caf_conn_new (int s, int f, socklen_t al, struct sockaddr *src,
	                          struct sockaddr *dst);
	int caf_conn_delete (caf_conn_t *c);
	int caf_conn_socket (caf_conn_t *c, int dom, int type, int proto);
	int caf_conn_options (caf_conn_t *c, caf_connection_mode_t mode, int lev,
	                      int cmd, void *opt);
	int caf_conn_options_get (caf_conn_t *c, caf_connection_mode_t mode, int lev,
	                          int cmd, void *opt, socklen_t *len);
	int caf_conn_options_clone (caf_conn_t *dst, caf_conn_t *src);
	int caf_conn_connect (caf_conn_t *c);
	int caf_conn_hardtcpc (caf_conn_t *c);
	ssize_t caf_conn_recv (caf_conn_t *c, cbuffer_t *b, int flg);
	ssize_t caf_conn_send (caf_conn_t *c, cbuffer_t *b, int flg);
	int caf_conn_bind (caf_conn_t *c);
	int caf_conn_listen (caf_conn_t *c, int bl);
	int caf_conn_accept (caf_conn_t *c);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_IO_NET_H */
/* caf_io_net.h ends here */
