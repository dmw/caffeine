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

  $Id$
*/
#ifndef CAF_DSO_H
#define CAF_DSO_H 1
/**
 * @defgroup      caf_dso             Caffeine Dynamic Shared Objects
 * @ingroup       caf_process
 * @addtogroup    caf_dso
 * @{
 *
 * @brief     Caffeine Dynamic Shared Objects
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine Dynamic Shared Objects
 *
 */

#include <caf/caf.h>
#include <caf/caf_data_lstdl.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define CAF_DSO_T_SZ				(sizeof(caf_dso_t))
#define CAF_DSO_TABLE_T_SZ			(sizeof(caf_dso_table_t))

typedef struct caf_dso_table_s caf_dso_table_t;
struct caf_dso_table_s {
	int id;
	int count;
	caf_hash_table_t *symt;
};

typedef struct caf_dso_s caf_dso_t;
struct caf_dso_s {
	int id;
	char *path;
	void *handle;
	caf_dso_table_t *table;
	void *(*load) (void);
};

#ifndef COMPILING_CAFFEINE
extern const char *CAF_DSO_INIT_LOADSYMS;
#endif /* !COMPILING_CAFFEINE */

caf_dso_t *caf_dso_open(const char *path, const int mode, const int id);
int caf_dso_close(caf_dso_t *dso);
void *caf_dso_sym(caf_dso_t *dso, const char *name);
caf_dso_table_t *caf_dso_table_new(const int id);
int caf_dso_table_delete(caf_dso_table_t *dsot);
int caf_dso_table_add(caf_dso_table_t *dsot, const char *name,
					  const void *what);
int caf_dso_table_remove(caf_dso_table_t *dsot, const char *name);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DSO_H */
/* caf_dso.h ends here */

