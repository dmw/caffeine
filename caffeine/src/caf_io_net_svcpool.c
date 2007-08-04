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

#include <sys/socket.h>
#include <unistd.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_io_file.h"
#include "caf/caf_io_net.h"
#include "caf/caf_io_net_svcpool.h"


static int caf_svcpool_delete_callback (void *data);

caf_svcpool_t *
caf_svcpool_new (int id, int num, caf_conn_t *seed) {
	caf_svcpool_t *r = (caf_svcpool_t *)NULL;
	if (id > 0 && num > 0 && seed != (caf_conn_t *)NULL) {
		r = (caf_svcpool_t *)xmalloc (CAF_SVCPOOL_SZ);
		if (r != (caf_svcpool_t *)NULL) {
			r->svc_num = num;
		}
	}
	return r;
}


int
caf_svcpool_delete (caf_svcpool_t *svc) {
	if (svc != (caf_svcpool_t *)NULL) {
		if ((lstdl_delete (svc->svc_lst, caf_svcpool_delete_callback)) ==
		        CAF_OK) {
			xfree (svc->svc_fds);
			xfree (svc);
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


static int
caf_svcpool_delete_callback (void *data) {
	caf_conn_t *con;
	if (data != (void *)NULL) {
		con = (caf_conn_t *)data;
		return caf_conn_delete (con);
	}
	return CAF_ERROR;
}


int
caf_svcpool_init (caf_svcpool_t *svc) {
	int *fds, c;
	caf_conn_t *i, *sc;
	if (svc != (caf_svcpool_t *)NULL) {
		if (svc->svc_seed != (caf_conn_t *)NULL) {
			svc->svc_fds = (int *)xmalloc ((size_t)svc->svc_num * sizeof (int));
			svc->svc_lst = lstdl_create ();
			if (svc->svc_fds != (int *)NULL &&
			        svc->svc_lst != (lstdl_t *)NULL) {
				memset (svc->svc_fds, (int)NULL,
				        (size_t)svc->svc_num * sizeof (int));
				fds = svc->svc_fds;
				sc = svc->svc_seed;
				for (c = 0; c < svc->svc_num; c++) {
					fds[c] = -1;
					fds[c] = dup (sc->sock);
					if (fds[c] >= 0) {
						i = caf_conn_new (fds[c], sc->flags, sc->addrlen,
						                  sc->saddr, sc->daddr);
						if (i != (caf_conn_t *)NULL) {
							i->flags = sc->flags;
							i->dom = sc->dom;
							i->type = sc->type;
							lstdl_push (svc->svc_lst, (void *)i);
						}
					} else {
						lstdl_push (svc->svc_lst, (void *)NULL);
					}
				}
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


int
caf_svcpool_stop (caf_svcpool_t *svc) {
	int c, fd, r;
	if (svc != (caf_svcpool_t *)NULL) {
		if (svc->svc_fds != (int *)NULL && svc->svc_lst != (lstdl_t *)NULL) {
			for (c = 0; c <= svc->svc_num; c++) {
				fd = svc->svc_fds[c];
				if (fd > -1) {
					r += shutdown (fd, SHUT_RDWR);
				}
			}
		}
	}
	return CAF_ERROR;
}


int
caf_svcpool_close (caf_svcpool_t *svc) {
	int c, fd, r;
	if (svc != (caf_svcpool_t *)NULL) {
		if (svc->svc_fds != (int *)NULL && svc->svc_lst != (lstdl_t *)NULL) {
			for (c = 0; c <= svc->svc_num; c++) {
				fd = svc->svc_fds[c];
				if (fd > -1) {
					r += close (fd);
				}
			}
		}
	}
	return CAF_ERROR;
}


int
caf_svcpool_finalize (caf_svcpool_t *svc) {
	int c, fd, r;
	if (svc != (caf_svcpool_t *)NULL) {
		if (svc->svc_fds != (int *)NULL && svc->svc_lst != (lstdl_t *)NULL) {
			for (c = 0; c <= svc->svc_num; c++) {
				fd = svc->svc_fds[c];
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
caf_svcpool_reopen (caf_svcpool_t *svc) {
	int c = 0, fd = -1, r = 0;
	lstdln_t *n;
	caf_conn_t *cc;
	if (svc != (caf_svcpool_t *)NULL) {
		if (svc->svc_fds != (int *)NULL && svc->svc_lst != (lstdl_t *)NULL &&
		        svc->svc_seed != (caf_conn_t *)NULL) {
			n = svc->svc_lst->head;
			c = 0;
			while (n != (lstdln_t *)NULL) {
				cc = (caf_conn_t *)n->data;
				if (svc->svc_fds[c] > -1) {
					if ((fd = dup (svc->svc_seed->sock)) > -1) {
						shutdown (cc->sock, SHUT_RDWR);
						close (cc->sock);
						cc->sock = fd;
						svc->svc_fds[c] = fd;
						r++;
					}
				} else {
					if ((svc->svc_fds[c] = dup (svc->svc_seed->sock)) > -1) {
						cc->sock = fd;
						svc->svc_fds[c] = fd;
						r++;
					}
				}
				n = n->next;
				c++;
			}
		}
	}
	return r;
}

/* caf_io_net_svcpool.c ends here */
