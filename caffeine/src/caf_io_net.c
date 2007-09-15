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
#ifndef lint
static char Id[] = "$Id$";
#endif /* !lint */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/fcntl.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_io_file.h"
#include "caf/caf_io_net.h"


static struct caf_sockopt_map {
	int opt;
	socklen_t sz;
}
caf_sockopt_map_v[] = {
	{ SO_DEBUG, (socklen_t)sizeof (int) },
	{ SO_REUSEADDR, (socklen_t)sizeof (int) },
#ifndef _GNU_SOURCE
	{ SO_REUSEPORT, (socklen_t)sizeof (int) },
#endif /* !_GNU_SOURCE */
	{ SO_KEEPALIVE, (socklen_t)sizeof (int) },
	{ SO_DONTROUTE, (socklen_t)sizeof (int) },
	{ SO_LINGER, (socklen_t)sizeof (struct linger) },
	{ SO_BROADCAST, (socklen_t)sizeof (int) },
	{ SO_OOBINLINE, (socklen_t)sizeof (int) },
	{ SO_SNDBUF, (socklen_t)sizeof (int) },
	{ SO_RCVBUF, (socklen_t)sizeof (int) },
	{ SO_SNDLOWAT, (socklen_t)sizeof (int) },
	{ SO_RCVLOWAT, (socklen_t)sizeof (int) },
	{ SO_SNDTIMEO, (socklen_t)sizeof (struct timeval) },
	{ SO_RCVTIMEO, (socklen_t)sizeof (struct timeval) },
#ifndef _GNU_SOURCE
	{ SO_ACCEPTFILTER, (socklen_t)sizeof (int) },
	{ SO_NOSIGPIPE, (socklen_t)sizeof (int) }
#endif /* !_GNU_SOURCE */
};


caf_conn_t *
caf_conn_new (int s, int f, socklen_t al, struct sockaddr *src,
              struct sockaddr *dst) {
	caf_conn_t *con = (caf_conn_t *)NULL;
	if (f > 0 && al > 0) {
		con = (caf_conn_t *)xmalloc (CAF_CONNECTION_SZ);
		if (con != (caf_conn_t *)NULL) {
			con->sock = s;
			con->flags = f;
			con->addrlen = al;
			con->saddr = src;
			con->daddr = dst;
		}
	}
	return con;
}


int
caf_conn_delete (caf_conn_t *c) {
	if (c != (caf_conn_t *)NULL) {
		if (c->flags & CAF_CONN_FREE_SRC) {
			if (c->saddr != (struct sockaddr *)NULL) {
				xfree (c->saddr);
			}
		}
		if (c->flags & CAF_CONN_FREE_DST) {
			if (c->daddr != (struct sockaddr *)NULL) {
				xfree (c->daddr);
			}
		}
		xfree (c);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_conn_socket (caf_conn_t *c, int dom, int type, int proto) {
	if (c != (caf_conn_t *)NULL) {
		if (dom > 0 && type > 0 && proto > 0) {
			c->sock = socket (dom, type, proto);
			if (c->sock >= 0) {
				c->dom = dom;
				c->type = type;
				c->proto = proto;
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


int
caf_conn_options (caf_conn_t *c, caf_connection_mode_t mode, int lev, int cmd,
                  void *opt) {
	int r = CAF_ERROR;
	int icmd = cmd;
	int iopt = *((int *)opt);
	int flg = 0;
	if (c != (caf_conn_t *)NULL) {
		if (mode > 0 && cmd > 0 && opt != (void *)NULL) {
			switch (mode) {
			case CAF_CONN_SOCKOPTS:
				if (lev > 0 ) {
					switch (cmd) {
					case SO_LINGER:
						r = setsockopt (c->sock, lev, cmd,
						                (struct linger *)opt,
						                sizeof (struct linger));
						break;
					case SO_SNDTIMEO:
					case SO_RCVTIMEO:
						r = setsockopt (c->sock, lev, cmd,
						                (struct timeval *)opt,
						                sizeof (struct timeval));
						break;
					default:
						r = setsockopt (c->sock, lev, cmd, (int *)opt,
						                sizeof (int));
						break;
					}
				}
				break;
			case CAF_CONN_FCNTL:
				if (cmd == F_SETFL) {
					flg = fcntl (c->sock, F_GETFL, 0);
					r = fcntl (c->sock, icmd, iopt | flg);
				} else {
					r = fcntl (c->sock, icmd, iopt);
				}
				break;
			default:
				break;
			}
		}
	}
	return r;
}


int
caf_conn_options_get (caf_conn_t *c, caf_connection_mode_t mode, int lev,
                      int cmd, void *opt, socklen_t *len) {
	int r = CAF_ERROR;
	int icmd = cmd;
	int iopt = *((int *)opt);
	if (c != (caf_conn_t *)NULL) {
		if (mode > 0 && cmd > 0 && opt != (void *)NULL) {
			switch (mode) {
			case CAF_CONN_SOCKOPTS:
				if (lev > 0 ) {
					r = getsockopt (c->sock, lev, cmd, opt, len);
				}
				break;
			case CAF_CONN_FCNTL:
				r = fcntl (c->sock, icmd, iopt);
				break;
			default:
				break;
			}
		}
	}
	return r;
}


int
caf_conn_options_clone (caf_conn_t *dst, caf_conn_t *src) {
	int r = CAF_ERROR, i, opt;
	struct linger li;
	struct timeval tv;
	int v;
	socklen_t l, sz;
	if (src != (caf_conn_t *)NULL && dst != (caf_conn_t *)NULL) {
		for (i = 0; i < (int)sizeof (caf_sockopt_map_v); i++) {
			opt = caf_sockopt_map_v[i].opt;
			sz = caf_sockopt_map_v[i].sz;
			switch (opt) {
			case SO_LINGER:
				r += caf_conn_options_get (src, CAF_CONN_SOCKOPTS, SOL_SOCKET,
				                           opt, (void *)&li, &l);
				r += caf_conn_options (dst, CAF_CONN_SOCKOPTS, SOL_SOCKET, opt,
				                       (void *)&li);
			case SO_SNDTIMEO:
			case SO_RCVTIMEO:
				r += caf_conn_options_get (src, CAF_CONN_SOCKOPTS, SOL_SOCKET,
				                           opt, (void *)&tv, &l);
				r += caf_conn_options (dst, CAF_CONN_SOCKOPTS, SOL_SOCKET, opt,
				                       (void *)&tv);
			default:
				r += caf_conn_options_get (src, CAF_CONN_SOCKOPTS, SOL_SOCKET,
				                           opt, (void *)&v, &l);
				r += caf_conn_options (dst, CAF_CONN_SOCKOPTS, SOL_SOCKET, opt,
				                       (void *)&v);
			}
		}
	}
	return r;
}

int
caf_conn_connect (caf_conn_t *c) {
	if (c != (caf_conn_t *)NULL) {
		return connect (c->sock, c->daddr, c->addrlen);
	}
	return CAF_ERROR;
}


int
caf_conn_hrdtcpcon (caf_conn_t *c) {
	int r = CAF_OK;
	int ra = 1;
	int ka = 1;
	int nd = 1;
	int nf = O_NONBLOCK;
	if (c != (caf_conn_t *)NULL) {
		if ((caf_conn_socket (c, PF_INET, SOCK_STREAM, IPPROTO_TCP)) ==
			CAF_OK) {
			r += caf_conn_options (c, CAF_CONN_SOCKOPTS, SOL_SOCKET,
			                       SO_REUSEADDR, &ra);
			r += caf_conn_options (c, CAF_CONN_SOCKOPTS, SOL_SOCKET,
			                       SO_KEEPALIVE, &ka);
			r += caf_conn_options (c, CAF_CONN_SOCKOPTS, IPPROTO_TCP,
			                       TCP_NODELAY, &nd);
			r += (caf_conn_options (c, CAF_CONN_FCNTL, CAF_CONN_NOLEVEL,
			                        F_SETFL, &nf)) == -1 ? CAF_ERROR : CAF_OK;
			if (r == CAF_OK) {
				return caf_conn_connect (c);
			}
		}
	}
	return CAF_ERROR;
}


int
caf_conn_hrdtcpsrv (caf_conn_t *c, int bl) {
	int r = CAF_OK;
	int ra = 1;
	int ka = 1;
	int nd = 1;
	int nf = O_NONBLOCK;
	if (c != (caf_conn_t *)NULL) {
		if ((caf_conn_socket (c, PF_INET, SOCK_STREAM, IPPROTO_TCP)) ==
			CAF_OK) {
			r += caf_conn_options (c, CAF_CONN_SOCKOPTS, SOL_SOCKET,
			                       SO_REUSEADDR, &ra);
			r += caf_conn_options (c, CAF_CONN_SOCKOPTS, SOL_SOCKET,
			                       SO_KEEPALIVE, &ka);
			r += caf_conn_options (c, CAF_CONN_SOCKOPTS, IPPROTO_TCP,
			                       TCP_NODELAY, &nd);
			r += (caf_conn_options (c, CAF_CONN_FCNTL, CAF_CONN_NOLEVEL,
			                        F_SETFL, &nf)) == -1 ? CAF_ERROR : CAF_OK;
			if (r == CAF_OK) {
				if ((bind (c->sock, c->daddr, c->addrlen)) == 0) {
					return (listen (c->sock, bl));
				}
			}
		}
	}
	return CAF_ERROR;
}


ssize_t
caf_conn_recv (caf_conn_t *c, cbuffer_t *b, int flg) {
	if (c != (caf_conn_t *)NULL && b != (cbuffer_t *)NULL) {
		b->iosz = recvfrom (c->sock, b->data, b->sz, flg, c->saddr,
		                    (socklen_t *)&(c->addrlen));
		return b->iosz;
	}
	return 0;
}


ssize_t
caf_conn_send (caf_conn_t *c, cbuffer_t *b, int flg) {
	if (c != (caf_conn_t *)NULL && b != (cbuffer_t *)NULL) {
		b->iosz = sendto (c->sock, b->data, (size_t)b->iosz, flg, c->daddr,
		                  c->addrlen);
		return b->iosz;
	}
	return 0;
}


int
caf_conn_bind (caf_conn_t *c) {
	if (c != (caf_conn_t *)NULL) {
		return bind (c->sock, c->daddr, c->addrlen);
	}
	return CAF_ERROR;
}


int
caf_conn_listen (caf_conn_t *c, int bl) {
	if (c != (caf_conn_t *)NULL) {
		return listen (c->sock, bl);
	}
	return CAF_ERROR;
}


int
caf_conn_accept (caf_conn_t *c) {
	if (c != (caf_conn_t *)NULL) {
		return accept (c->sock, c->saddr, (socklen_t *)&(c->addrlen));
	}
	return CAF_ERROR;
}


/* caf_io_net.c ends here */
