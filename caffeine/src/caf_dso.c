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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstdl.h"
#include "caf/caf_hash_str.h"
#include "caf/caf_hash_table.h"
#include "caf/caf_dso.h"


const char CAF_DSO_INIT_LOADSYMS[] = "CAF_DSO_INIT_LOADSYMS";


caf_dso_t *
caf_dso_open (const char *path, const int mode, const int id) {
	caf_dso_t *r = (caf_dso_t *)NULL;
	if (path != (const char *)NULL && id > 0) {
		r = (caf_dso_t *)xmalloc (CAF_DSO_T_SZ);
		if (r != (caf_dso_t *)NULL) {
			r->id = id;
			r->path = strdup (path);
			r->handle = dlopen (path, mode);
			if (r->handle != NULL) {
				r->load = (void *(*)(void))dlsym (r->handle,
												  CAF_DSO_INIT_LOADSYMS);
				if (r->load != (void *(*)(void))NULL) {
					r->table = (caf_dso_table_t *)r->load ();
				}
			}
		}
	}
	return r;
}


int
caf_dso_close (caf_dso_t *dso) {
	int r = CAF_ERROR;
	if (dso != (caf_dso_t *)NULL) {
		if ((dlclose (dso->handle)) == 0) {
			if ((caf_dso_table_delete (dso->table)) == CAF_OK) {
				xfree (dso->path);
				xfree (dso);
				r = CAF_OK;
			}
		}
	}
	return r;
}


void *
caf_dso_sym (caf_dso_t *dso, const char *name) {
	void *r = (void *)NULL;
	if (dso != (caf_dso_t *)NULL) {
		if (dso->table != (caf_dso_table_t *)NULL) {
			if (dso->table->symt != (caf_hash_table_t *)NULL) {
				r = caf_hash_table_get (dso->table->symt,
										(const void *)name,
										(const size_t)(strlen (name) + 1));
			}
		}
	}
	return r;
}


caf_dso_table_t *
caf_dso_table_new (const int id) {
	caf_dso_table_t *tbl = (caf_dso_table_t *)NULL;
	if (id > 0) {
		tbl = (caf_dso_table_t *)xmalloc (CAF_DSO_TABLE_T_SZ);
		if (tbl != (caf_dso_table_t *)NULL) {
			tbl->id = id;
			tbl->count = 0;
			tbl->symt = caf_hash_table_new (id, caf_shash_bp,
											caf_shash_dek);
			if (tbl->symt == (caf_hash_table_t *)NULL) {
				xfree (tbl);
				tbl = (caf_dso_table_t *)NULL;
			}
		}
	}
	return tbl;
}


int
caf_dso_table_delete (caf_dso_table_t *dsot) {
	int r = CAF_ERROR;
	if (dsot != (caf_dso_table_t *)NULL) {
		if ((caf_hash_table_delete (dsot->symt)) == CAF_OK) {
			xfree (dsot);
			r = CAF_OK;
		}
	}
	return r;
}


int
caf_dso_table_add (caf_dso_table_t *dsot, const char *name,
				   const void *what) {
	int r = CAF_ERROR;
	if (dsot != (caf_dso_table_t *)NULL) {
		if (dsot->symt != (caf_hash_table_t *)NULL) {
			if ((caf_hash_table_add (dsot->symt, (const void *)name,
									 (const size_t)(strlen (name) + 1),
									 (const void *)what))
				== CAF_OK) {
				dsot->count++;
				r = CAF_OK;
			}
		}
	}
	return r;
}


int
caf_dso_table_remove (caf_dso_table_t *dsot, const char *name) {
	int r = CAF_ERROR;
	if (dsot != (caf_dso_table_t *)NULL) {
		if (dsot->symt != (caf_hash_table_t *)NULL) {
			if ((caf_hash_table_remove (dsot->symt, (const void *)name,
										(const size_t)(strlen (name) + 1)))
				== CAF_OK) {
				dsot->count++;
				r = CAF_OK;
			}
		}
	}
	return r;
}


/* caf_dso.c ends here */
