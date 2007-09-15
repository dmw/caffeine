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
#include <pcre.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstdl.h"
#include "caf/caf_regex_pcre.h"


static int regex_pcre_delete_callback (void *re);


regex_pcre_t *
regex_pcre_new (const int id, const char *pat, const int opt,
                const unsigned char *tbl, const int study) {
	regex_pcre_t *re;
	char *study_error;
	re = (regex_pcre_t *)NULL;
	if (id > 0 && pat != (char *)NULL) {
		re = (regex_pcre_t *)xmalloc (REGEX_PCRE_T_SZ);
		if (re != (regex_pcre_t *)NULL) {
			re->erroff = 0;
			re->errstr = (char *)NULL;
			re->code = pcre_compile (pat, opt, &(re->errstr), &(re->erroff),
			                         tbl);
			if (re->code == (pcre *)NULL) {
				xfree (re);
				re = (regex_pcre_t *)NULL;
			} else {
				if (study == CAF_OK) {
					re->study = pcre_study (re->code, 0, &study_error);
				} else {
					re->study = (pcre_extra *)NULL;
				}
				re->opt = opt;
				re->pattern = pat;
			}
		}
	}
	return re;
}


int
regex_pcre_delete (regex_pcre_t *re) {
	if (re != (regex_pcre_t *)NULL) {
		if (re->code != (pcre *)NULL) {
			pcre_free (re->code);
		}
		if (re->study != (pcre_extra *)NULL) {
			pcre_free (re->extra);
		}
		xfree (re);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
regex_pcre_match (regex_pcre_t *re, const char *sub) {
	int ov[REGEX_PCRE_OV_SZ];
	if (re != (regex_pcre_t *)NULL && sub != (char *)NULL) {
		memset (ov, (int)NULL, REGEX_PCRE_OV_SZ);
		if ((pcre_exec (re->code, re->study, sub, strlen (sub), 0, 0, ov,
		                REGEX_PCRE_OV_SZ)) > 0) {
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


int
regex_pcre_find (regex_pcre_t *re, const char *sub, int *ov) {
	if (re != (regex_pcre_t *)NULL && sub != (char *)NULL) {
		memset (ov, (int)NULL, REGEX_PCRE_OV_SZ);
		return pcre_exec (re->code, re->study, sub, strlen (sub), 0, 0, ov,
		                  REGEX_PCRE_OV_SZ);
	}
	return CAF_ERROR_SUB;
}


int
regex_pcre_subs (regex_pcre_t *re, const char *sub, cbuffer_t *out) {
	int c, r;
	int ov[REGEX_PCRE_OV_SZ];
	if (re != (regex_pcre_t *)NULL && sub != (char *)NULL &&
		out != (cbuffer_t *)NULL) {
		r = regex_pcre_find (re, sub);
		if (r > 0) {
			for (c = 0; c < r; c++) {
				pcre_copy_substring (sub, &r, r, c, (char *)out->data,
				                     out->sz);
			}
		} else {
			return CAF_OK;
		}
	}
	return CAF_ERROR_SUB;
}


regex_pcre_pool_t *
regex_pcre_pool_new (const int id, const char *name) {
	regex_pcre_pool_t *r = (regex_pcre_pool_t *)NULL;
	if (id > 0 && name != (const char *)NULL) {
		r = (regex_pcre_pool_t *)xmalloc (REGEX_PCRE_POOL_SZ);
		if (r != (regex_pcre_pool_t *)NULL) {
			r->pool = lstdl_create ();
			if (r->pool == (lstdl_t *)NULL) {
				xfree (r);
				r = (regex_pcre_pool_t *)NULL;
			}
		}
	}
	return r;
}


int
regex_pcre_pool_delete (regex_pcre_pool_t *p) {
	if (p != (regex_pcre_pool_t *)NULL) {
		if (p->pool != (lstdl_t *)NULL) {
			lstdl_delete (p->pool, regex_pcre_delete_callback);
			xree (p);
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


int
regex_pcre_pool_match (regex_pcre_pool_t *p, const char *sub) {
	lstdln_t *n;
	regex_pcre_t *re;
	if (p != (regex_pcre_pool_t *)NULL) {
		if (p->pool != (lstdl_t *)NULL) {
			n = p->pool->frst;
			if (n != (lstdln_t *)NULL) {
				while (n != (lstdln_t *)NULL) {
					re = (regex_pcre_t *)n->data;
					if ((regex_pcre_match (re, sub)) == CAF_OK) {
						return re->id;
					}
					n = n->next;
				}
			}
		}
	}
	return CAF_ERROR_SUB;
}


static int
regex_pcre_delete_callback (void *re) {
	return regex_pcre_delete ((regex_pcre_t *)re);
}


/* caf_regex_pcre.c ends here */
