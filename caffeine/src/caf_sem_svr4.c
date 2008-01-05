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

#ifdef HAVE_CONFIG_H
#include "caf/config.h"
#endif /* !HAVE_CONFIG_H */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstdl.h"
#include "caf/caf_sem_svr4.h"

caf_sem_svr4_t *
caf_sem_svr4_new (key_t key, const int nsems, const int flag) {
	caf_sem_svr4_t *r = (caf_sem_svr4_t *)NULL;
	if ((int)key >= 0 && nsems > 0 && flag > 0) {
		r = (caf_sem_svr4_t *)xmalloc (CAF_SEM_SVR4_SZ);
		if (r != (caf_sem_svr4_t *)NULL) {
			r->id = semget (key, nsems, flag | IPC_CREAT);
			if (r->id >= 0) {
				r->key = key;
				r->nsems = nsems;
				r->flag = flag;
				return r;
			} else {
				xfree (r);
				r = (caf_sem_svr4_t *)NULL;
			}
		}
	}
	return r;
}

int
caf_sem_svr4_delete (caf_sem_svr4_t *r) {
	if ((caf_sem_svr4_exists (r)) == 0) {
		if ((semctl (r->id, 0, IPC_RMID, NULL)) == 0) {
			xfree (r);
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


caf_sem_svr4_t *
caf_sem_svr4_get (const key_t k, const int ns, const int f) {
	caf_sem_svr4_t *r = (caf_sem_svr4_t *)NULL;
	int ri = semget (k, ns, f);
	if (ri != -1) {
		r = (caf_sem_svr4_t *)xmalloc (CAF_SEM_SVR4_SZ);
		if (r != (caf_sem_svr4_t *)NULL) {
			r->id = ri;
			r->key = k;
			r->nsems = ns;
			r->flag = f;
		}
	}
	return r;
}


int
caf_sem_svr4_exists (caf_sem_svr4_t *r) {
	int i, c = 0;
	if (r != (caf_sem_svr4_t *)NULL) {
		if (r->id >= 0) {
			for (i = 0; i < r->nsems; i++) {
				if ((semget (r->key, i, r->flag)) <= 0) {
					c++;
				}
			}
			return (c > 0 ? CAF_OK : CAF_ERROR);
		}
	}
	return CAF_ERROR;
}


int
caf_sem_svr4_op (caf_sem_svr4_t *r, caf_sembuf_svr4_t *op) {
	if (r != (caf_sem_svr4_t *)NULL && op != (caf_sembuf_svr4_t *)NULL) {
		if ((caf_sem_svr4_exists (r)) == 0) {
			return semop (r->id, op->ops, op->nopsz);
		}
	}
	return CAF_ERROR_SUB;
}


int
caf_sem_svr4_ctl (caf_sem_svr4_t *r, int n, int cmd, void *a) {
	int oen;
	int *i = (int *)a;
	if (r != (caf_sem_svr4_t *)NULL) {
		switch (cmd) {
		case GETVAL:
		case GETPID:
		case GETNCNT:
		case GETZCNT:
			oen = errno;
			errno = 0;
			*i = semctl (r->id, n, cmd);
			if (errno != 0) {
				errno = oen;
				return CAF_ERROR;
			} else {
				errno = oen;
				return CAF_OK;
			}
			break;
		default:
			return semctl (r->id, n, cmd, (union semun *)a);
			break;
		}
	}
	return CAF_OK;
}


caf_sembuf_svr4_t *
caf_sembuf_svr4_new (caf_sem_svr4_t *r, const short op, const short flg,
                     struct timespec *to) {
	size_t i = 0, nopsz = 0;
	struct sembuf *ptr = (struct sembuf *)NULL;
	caf_sembuf_svr4_t *b = (caf_sembuf_svr4_t *)NULL;
	if (r != (caf_sem_svr4_t *)NULL) {
		b = (caf_sembuf_svr4_t *)xmalloc (CAF_SEMBUF_SVR4_SZ);
		if (b != (caf_sembuf_svr4_t *)NULL) {
			b->nopsz = (size_t)r->nsems * CAF_SEM_BUF_SZ;
			b->ops = (struct sembuf *)xmalloc (nopsz);
			b->to = to;
			if (b->ops != (struct sembuf *)NULL) {
				for (i = 0; i < (size_t)r->nsems; i++) {
					ptr = (struct sembuf *)(i + (size_t)b->ops);
					ptr->sem_num = (u_short)i;
					ptr->sem_op = (short)op;
					ptr->sem_flg = (short)flg;
				}
				return b;
			} else {
				xfree (b);
				b = (caf_sembuf_svr4_t *)NULL;
			}
		}
	}
	return b;
}


int
caf_sembuf_svr4_delete (caf_sembuf_svr4_t *b) {
	if (b != (caf_sembuf_svr4_t *)NULL) {
		if (b->ops != (struct sembuf *)NULL) {
			xfree (b->ops);
		}
		if (b->to != (struct timespec *)NULL) {
			xfree (b->to);
		}
		xfree (b);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_sem_svr4_lock (caf_sem_svr4_t *r) {
	int i;
	struct sembuf *sb;
	if (r != (caf_sem_svr4_t *)NULL) {
		sb = (struct sembuf *)xmalloc (sizeof (struct sembuf));
		if (sb != (struct sembuf *)NULL) {
			for (i = 0; i < r->nsems; i++) {
				sb[i].sem_num = i;
				sb[i].sem_op = (u_short)CAF_SEM_SVR4_LOCK;
				sb[i].sem_flg = (u_short)NULL;
			}
			i = semop (r->id, sb, (size_t)r->nsems);
			xfree (sb);
			if (i == 0) {
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


int
caf_sem_svr4_trylock (caf_sem_svr4_t *r) {
	return caf_sem_svr4_lock (r);
}


int
caf_sem_svr4_unlock (caf_sem_svr4_t *r) {
	int i;
	struct sembuf *sb;
	if (r != (caf_sem_svr4_t *)NULL) {
		sb = (struct sembuf *)xmalloc (sizeof (struct sembuf));
		if (sb != (struct sembuf *)NULL) {
			for (i = 0; i < r->nsems; i++) {
				sb[i].sem_num = i;
				sb[i].sem_op = (u_short)CAF_SEM_SVR4_UNLOCK;
				sb[i].sem_flg = (u_short)NULL;
			}
			i = semop (r->id, sb, (size_t)r->nsems);
			xfree (sb);
			if (i == 0) {
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}

/* caf_sem_svr4.c ends here */

