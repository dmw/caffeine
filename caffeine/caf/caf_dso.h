/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "caf" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
/*
  Caffeine - C Application Framework
  Copyright (C) 2006 Daniel Molina Wegener <dmw@coder.cl>

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

#include <dlfcn.h>

#ifdef HAVE_CONFIG_H
#include <caf/config.h>
#endif /* HAVE_CONFIG_H */

#include <caf/caf.h>
#include <caf/caf_data_lstdl.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** sizeof applied to caf_dso_t  */
#define CAF_DSO_T_SZ				(sizeof(caf_dso_t))
/** sizeof applied to caf_dso_table_t */
#define CAF_DSO_TABLE_T_SZ			(sizeof(caf_dso_table_t))


typedef void *(*caf_function_t)(void);
typedef void *caf_object_t;


/**
 * @brief		Caffeine DSO Table
 *
 * Caffeine DSO Table contains the symbol table using a Caffeine
 * hashtable where contains a symbol count and you can access
 * symbols by using DSO Table interfaces.
 */
typedef struct caf_dso_table_s caf_dso_table_t;
/**
 * @brief		Caffeine DSO Table Structure
 *
 * The table structure for DSOs.
 */
struct caf_dso_table_s {
	/** Table identifier */
	int id;
	/** Symbol Count */
	int count;
	/** Symbol Table */
	caf_hash_table_t *symt;
};

/**
 * @brief		Caffeine DSO Object
 *
 * Caffeine DSO Object contains a complete definition for DSOs
 * using a DSO Table member to access symbols and the load
 * routine is called on DSO allocation. You must define the
 * routine returning a Caffeine Hashtable containing the
 * symbol table.
 *
 * @see caf_dso_s
 */
typedef struct caf_dso_s caf_dso_t;
/**
 * @brief		Caffeine DSO Object Structure
 *
 * Defines the DSO Object structure.
 *
 * @see caf_dso_t
 */
struct caf_dso_s {
	/** Oject identifier */
	int id;
	/** DSO path */
	char *path;
	/** DSO handle */
	void *handle;
	/** DSO symbol table */
	caf_dso_table_t *table;
	/** DSO load routine, points to @see CAF_DSO_INIT_LOADSYMS */
	caf_function_t *load;
};

#ifndef COMPILING_CAFFEINE
/** Caffeine startup symbol to search for on Caffine DSOs */
extern const char *CAF_DSO_INIT_LOADSYMS;
#endif /* !COMPILING_CAFFEINE */


/**
 * @brief				Opens a DSO.
 *
 * Opens a DSO, calling CAF_DSO_INIT_LOADSYMS_STRING to load the
 * symbol table from the DSO. It's assumed that the DSO contains
 * a CAF_DSO_INIT_LOADSYMS_STRING definition and returns a symbol
 * table in the form of Caffine Hashtable.
 *
 * @param path			Path to the DSO to open.
 * @param mode			dlopen(3) mode.
 * @param id			DSO identifier.
 *
 * @return a new allocated caf_dso_t pointer on success, NULL on failure.
 */
caf_dso_t *caf_dso_open (const char *path, const int mode, const int id);

/**
 * @brief				Close a DSO.
 *
 * Close a DSO, deallocating a DSO and unloading DSO symbols. Also
 * deallocates the Caffeine Hashtable allocated for symbols.
 *
 * @param dso			DSO to deallocate.
 *
 * @return CAF_OK on success and CAF_ERROR on failure.
 */
int caf_dso_close (caf_dso_t *dso);

/**
 * @brief				Search a symbol using the local hashtable.
 *
 * Search a symbol in the local hashtable.
 *
 * @param dso			DSO where to search.
 * @param name			symbol name.
 *
 * @return a void pointer to the symbol, NULL on failure.
 */
void *caf_dso_sym (caf_dso_t *dso, const char *name);

/**
 * @brief				Search a symbol using dlsym(3).
 *
 * Search a symbol in the DSO handle, using dlsym(3) and
 * replaces the symbol definition in the local hashtable.
 *
 * @param dso			DSO where to search.
 * @param name			symbol name.
 *
 * @return a void pointer to the symbol, NULL on failure.
 */
void *caf_dso_dlsym (caf_dso_t *dso, const char *name);

/**
 * @brief				Search a symbol using dlfunc(3).
 *
 * Search a symbol in the DSO handle, using dlfunc(3) and
 * replaces the symbol definition in the local hashtable.
 *
 * @param dso			DSO where to search.
 * @param name			symbol name.
 *
 * @return a void pointer to the symbol, NULL on failure.
 */
caf_function_t *caf_dso_dlfunc (caf_dso_t *dso, const char *name);

/**
 * @brief				Wrapper for dlerror(3).
 *
 * Wrapper interface to dlerror(3).
 *
 * @return a pointer to the last dlerror(3) string.
 */
const char *caf_dso_dlerror ();

/**
 * @brief				Allocates a new DSO Table.
 *
 * Allocates a new DSO Table.
 *
 * @return a new allocated caf_dso_table_t pointer.
 */
caf_dso_table_t *caf_dso_table_new (const int id);

/**
 * @brief				Deallocates the given DSO Table.
 *
 * Deallocates the given DSO Table.
 *
 * @param dsot			Table to deallocate.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_dso_table_delete (caf_dso_table_t *dsot);

/**
 * @brief				Adds a new symbol to the table.
 *
 * Adds the given symbol to the table.
 *
 * @param dsot			Symbol table to deallocate
 * @param name			Symbol name
 * @param what			Symbol pointer
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_dso_table_add (caf_dso_table_t *dsot, const char *name,
					   const void *what);

/**
 * @brief				Removes a symbol the table.
 *
 * Removes the given symbol from the table using the symbol
 * name.
 *
 * @param dsot			Symbol table to deallocate
 * @param name			Symbol name
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_dso_table_remove (caf_dso_table_t *dsot, const char *name);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DSO_H */
/* caf_dso.h ends here */

