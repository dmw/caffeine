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
#include <errno.h>
#include <pthread.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_thread_key.h"


pth_key_t *
pth_key_new (int id, PTH_KEY_DESTROY_RTN(rtn)) {
	pth_key_t *k = (pth_key_t *)NULL;
	k = (pth_key_t *)xmalloc (PTH_KEY_T_SZ);
	if (id > 0 && k != (pth_key_t *)NULL) {
		k->id = id;
		k->rtn = rtn;
		if (pthread_key_create ((pthread_key_t *)&(k->key), rtn) != 0) {
			xfree (k);
			k = (pth_key_t *)NULL;
		}
	}
	return k;
}


int
pth_key_delete (pth_key_t *k) {
	int er = CAF_ERROR;
	if (k != (pth_key_t *)NULL) {
		if (k->rtn == NULL) {
			xfree (k->ptr);
		}
		er = pthread_key_delete (k->key);
		xfree (k);
	}
	return er;
}


int
pth_key_set (pth_key_t *key, size_t sz) {
	int er = CAF_ERROR;
	if (key != (pth_key_t *)NULL && sz > 0) {
		key->ptr = xmalloc (sz);
		if (key->ptr != (void *)NULL) {
			key->sz = sz;
			return pthread_setspecific (key->key, key->ptr);
		}
	}
	return er;
}


void *
pth_key_get (pth_key_t *key) {
	void *g = (void *)NULL;
	if (key != (pth_key_t *)NULL) {
		return pthread_getspecific (key->key);
	}
	return g;
}


pth_kpool_t *
pth_kpool_new (int id, pthread_t *thr) {
	pth_kpool_t *res = (pth_kpool_t *)NULL;
	if (id > 0) {
		res = (pth_kpool_t *)xmalloc (PTH_KPOOL_T_SZ);
		if (res != (pth_kpool_t *)NULL) {
			res->id = id;
			res->thr = thr;
			res->keys = lstdl_create ();
		}
	}
	return res;
}


void
pth_kpool_delete (pth_kpool_t *kpool) {
	if (kpool != (pth_kpool_t *)NULL) {
		lstdl_delete (kpool->keys, pth_kpool_remove_callback);
		xfree (kpool);
		kpool = (pth_kpool_t *)NULL;
	}
}


void *
pth_kpool_add (pth_kpool_t *pool, pth_key_t *key) {
	if (pool != (pth_kpool_t *)NULL) {
		if (key != (pth_key_t *)NULL) {
			return lstdl_push(pool->keys, (void *)key);
		}
	}
	return (void *)NULL;
}


pth_key_t *
pth_kpool_get_key (pth_kpool_t *pool, int id) {
	pth_key_t *k = (pth_key_t *)NULL;
	lstdln_t *n;
	if (pool != (pth_kpool_t *)NULL) {
		if (pool->keys != (lstdl_t *)NULL) {
			n = pool->keys->head;
			if (n != (lstdln_t *)NULL) {
				while (n != (lstdln_t *)NULL) {
					k = (pth_key_t *)n->data;
					if (k->id == id) {
						return k;
					}
					n = n->next;
				}
			}
		}
	}
	return k;
}


void *
pth_kpool_get (pth_kpool_t *pool, int id) {
	pth_key_t *k = (pth_key_t *)NULL;
	lstdln_t *n;
	if (pool != (pth_kpool_t *)NULL) {
		if (pool->keys != (lstdl_t *)NULL) {
			n = pool->keys->head;
			if (n != (lstdln_t *)NULL) {
				while (n != (lstdln_t *)NULL) {
					k = (pth_key_t *)n->data;
					if (k->id == id) {
						return pth_key_get (k);
					}
					n = n->next;
				}
			}
		}
	}
	return (void *)NULL;
}


void
pth_kpool_remove_by_id (pth_kpool_t *pool, int id) {
	pth_key_t *k = (pth_key_t *)NULL;
	lstdln_t *n;
	if (pool != (pth_kpool_t *)NULL) {
		if (pool->keys != (lstdl_t *)NULL) {
			n = pool->keys->head;
			if (n != (lstdln_t *)NULL) {
				while (n != (lstdln_t *)NULL) {
					k = (pth_key_t *)n->data;
					if (k->id == id) {
						lstdl_node_delete (pool->keys, n,
						                   pth_kpool_remove_callback);
					}
					n = n->next;
				}
			}
		}
	}
}


void
pth_kpool_remove (pth_kpool_t *pool, pth_key_t *key) {
	if (pool != (pth_kpool_t *)NULL && key != (pth_key_t *)NULL) {
		if (pool->keys != (lstdl_t *)NULL) {
			lstdl_node_delete_by_data (pool->keys, (void *)key,
			                           pth_kpool_remove_callback);
		}
	}
}


int
pth_kpool_remove_callback (void *k) {
	pth_key_t *kp = k;
	if (k != (void *)NULL) {
		return pth_key_delete (kp);
	}
	return 1;
}

/* caf_thread_key.c ends here */
