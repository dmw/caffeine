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
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_thread_attr.h"
#include "caf/caf_thread_pool.h"


pth_pool_t *
pth_pool_new (pth_attri_t *attrs, CAF_PT_PROTOTYPE(rtn), int count) {
	pth_pool_t *ptp = (pth_pool_t *)NULL;
	pthread_t *thr = (pthread_t *)NULL;
	int rc = 0;
	int c = 0;
	if (attrs != (pth_attri_t *)NULL) {
		ptp = (pth_pool_t *)xmalloc (CAF_PT_POOL_SZ);
		if (ptp != (pth_pool_t *)NULL && count > 0) {
			ptp->attri = attrs;
			ptp->rtn = rtn;
			ptp->threads = lstdl_create ();
			for (c = 1; c <= count; c++) {
				thr = (pthread_t *)xmalloc (sizeof(pthread_t));
				if (thr != (pthread_t *)NULL) {
					if ((lstdl_push (ptp->threads, thr)) != (lstdl_t *)NULL) {
						rc++;
					}
				}
			}
			ptp->count = rc;
		}
	}
	return ptp;
}


void
pth_pool_delete (pth_pool_t *p) {
	if (p != (pth_pool_t *)NULL) {
		if (p->threads != (lstdl_t *)NULL) {
			lstdl_delete (p->threads, pth_pool_delete_callback);
		}
		xfree (p);
		p = (pth_pool_t *)NULL;
	}
}


int
pth_pool_delete_callback (void *ptr) {
	int res = 0;
	pthread_t *pth = (pthread_t *)ptr;
	if (pth != (pthread_t *)NULL) {
		res = pthread_cancel (*pth);
		xfree (ptr);
		ptr = (void *)NULL;
		return CAF_OK;
	}
	return CAF_ERROR;
}


pth_pool_t *
pth_pool_create (pth_attri_t *attrs, CAF_PT_PROTOTYPE(rtn), int cnt, void *arg) {
	pth_pool_t *pool = (pth_pool_t *)NULL;
	int rt = 0;
	lstdln_t *n;
	pthread_t *thr;
	if (rtn != NULL && cnt > 0) {
		pool = pth_pool_new (attrs, rtn, cnt);
		if (pool != (pth_pool_t *)NULL) {
			if (pool->threads != (lstdl_t *)NULL) {
				n = pool->threads->head;
				if (n != (lstdln_t *)NULL) {
					while (n != (lstdln_t *)NULL) {
						thr = (pthread_t *)n->data;
						rt = pthread_create (thr, &(pool->attri->attr), rtn,
						                     arg);
						n = n->next;
					}
				}
			}
		}
	}
	return pool;
}


int
pth_pool_add (pth_pool_t *p, pth_attri_t *attrs, CAF_PT_PROTOTYPE(rtn),
              void *arg) {
	pth_pool_t *pool = (pth_pool_t *)NULL;
	int rt = CAF_ERROR;
	pthread_t *thr;
	if (rtn != NULL) {
		if (p != (pth_pool_t *)NULL) {
			if (p->threads != (lstdl_t *)NULL) {
				thr = (pthread_t *)xmalloc (sizeof (pthread_t));
				if (thr != (pthread_t *)NULL) {
					if (attrs != (pth_attri_t *)NULL) {
						rt = pthread_create (thr, &(attrs->attr), rtn,
						                     arg);
					} else {
						rt = pthread_create (thr, &(pool->attri->attr), rtn,
						                     arg);
					}
					if (rt == 0) {
						lstdl_push (pool->threads, thr);
					}
				}
			}
		}
	}
	return rt;
}


int
pth_pool_join (pth_pool_t *pool) {
	pth_attri_t *attri = (pth_attri_t *)NULL;
	int rt = 0, ds = 0, final = 0, jn = 0;
	void *stat = (void *)NULL;
	lstdln_t *n;
	pthread_t *thr;
	if (pool != (pth_pool_t *)NULL) {
		if (pool->threads != (lstdl_t *)NULL) {
			attri = (pth_attri_t *)pool->attri;
			if (attri->at & PTH_ATTR_JOINABLE) {
				jn = pth_attri_set(attri, PTH_ATTR_JOINABLE, (void *)&ds);
				if (jn == 0) {
					n = pool->threads->head;
					if (n != (lstdln_t *)NULL) {
						while (n != (lstdln_t *)NULL) {
							thr = (pthread_t *)n->data;
							stat = (void *)NULL;
							rt = pthread_join (*thr, (void **)&stat);
							if (stat != PTHREAD_CANCELED) {
								final += rt;
							}
							n = n->next;
						}
					}
				}
			}
		}
	}
	return final;
}


int
pth_pool_detach (pth_pool_t *pool) {
	pth_attri_t *attri = (pth_attri_t *)NULL;
	int rt = 0, ds = 0, final = 0, jn = 0;
	lstdln_t *n;
	pthread_t *thr;
	if (pool != (pth_pool_t *)NULL) {
		if (pool->threads != (lstdl_t *)NULL) {
			attri = (pth_attri_t *)pool->attri;
			if (attri->at & PTH_ATTR_JOINABLE) {
				jn = pth_attri_set(attri, PTH_ATTR_JOINABLE, (void *)&ds);
				if (jn != 0) {
					n = pool->threads->head;
					if (n != (lstdln_t *)NULL) {
						while (n != (lstdln_t *)NULL) {
							thr = (pthread_t *)n->data;
							rt = pthread_detach (*thr);
							final += rt;
							n = n->next;
						}
					}
				}
			}
		}
	}
	return final;
}


int
pth_pool_cancel (pth_pool_t *pool) {
	int rt = 0, final = 0;
	lstdln_t *n;
	pthread_t *thr;
	if (pool != (pth_pool_t *)NULL) {
		if (pool->threads != (lstdl_t *)NULL) {
			n = pool->threads->head;
			if (n != (lstdln_t *)NULL) {
				while (n != (lstdln_t *)NULL) {
					thr = (pthread_t *)n->data;
					rt = pthread_cancel (*thr);
					final += rt;
					n = n->next;
				}
			}
		}
	}
	return final;
}

/* caf_thread_pool.c ends here */
