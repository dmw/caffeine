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
#ifndef CAF_DATA_VAR_H
#define CAF_DATA_VAR_H 1
/**
 * @defgroup      caf_data_var        Variable Manager
 * @ingroup       caf_mem
 * @addtogroup    caf_data_var
 * @{
 *
 * @brief     Caffeine Variable Manager.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine Variable Management engine. This engine is used to get and set
 * Caffeine configuration variables. You can create, delete, set, get and
 * cast configuration variables through this set of functions.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#define CAF_VARIABLE_SZ        (sizeof(cafvar_t))
#define CAF_VARSPACE_SZ        (sizeof(cafvarspace_t))

typedef struct caf_variable_space cafvarspace_t;
struct caf_variable_space {
	int id;
	char *name;
	caflst_t *vars;
};

typedef struct caf_variable cafvar_t;
struct caf_variable {
	int type;
	char *name;
	void *value;
};

enum caf_vartypes
{
	CAFV_BOOLEAN = 10,
	CAFV_BYTE = 20,
	CAFV_CHAR = 30,
	CAFV_UCHAR = 40,
	CAFV_WCHAR = 50,
	CAFV_WUCHAR = 60,
	CAFV_SHORT = 70,
	CAFV_INT = 80,
	CAFV_UINT = 90,
	CAFV_LONG = 100,
	CAFV_ULONG = 110,
	CAFV_FLOAT = 120,
	CAFV_DOUBLE = 130,
	CAFV_STRING = 200
};

#define    CAFV_NATIVE_BOOLEAN        int
#define    CAFV_NATIVE_BYTE           unsigned int:8
#define    CAFV_NATIVE_CHAR           char
#define    CAFV_NATIVE_UCHAR          unsigned char
#define    CAFV_NATIVE_WCHAR          w_char_t
#define    CAFV_NATIVE_WUCHAR         unsigned w_char_t
#define    CAFV_NATIVE_SHORT          short
#define    CAFV_NATIVE_INT            int
#define    CAFV_NATIVE_UINT           unsigned int
#define    CAFV_NATIVE_LONG           long
#define    CAFV_NATIVE_ULONG          unsigned long
#define    CAFV_NATIVE_FLOAT          float
#define    CAFV_NATIVE_DOUBLE         double
#define    CAFV_NATIVE_STRING         char *


/**
 *
 * @brief    Creates a new CAF variable space.
 *
 * Creates a new CAF variable space (allocates memory).
 *
 * @param[in]    int            the space id.
 * @param[in]    char *         space name.
 * @return       cafvarspace_t *     the allocated variable space.
 *
 * @see      cafvarspace_delete
 */
cafvarspace_t *cafvarspace_new (int id, char *name);

/**
 *
 * @brief    Deallocates a variable space and his variables.
 *
 * Deletes or deallocates the given CAF variable space.
 *
 * @param[in]    cafvarspace_t *        the variable space.
 * @return       void
 *
 * @see      cafvarspace_new
 */
cafvar_t *cafvar_new (char *name, char *value, int type);

/**
 *
 * @brief    Allocates a new variable.
 *
 * Allocates a new variable using a void pointer as reference
 * for the data contained.
 *
 * @param[in]    name *                the variable name.
 * @param[in]    void *                the variable data.
 * @param[in]    int                   the variable type.
 * @return       cafvar_t *
 *
 * @see      cafvar_delete
 */
void cafvar_delete (cafvar_t *var);
void cafvar_dump (FILE *out, void *var);

void cafvar_init (void);
void cafvar_finish (void);
void cafvar_dump (FILE *out);
int cafvar_destroycb (void *data);
int cafvar_add (char *name, char *value, int type);
cafvar_t *cafvar_get (char *name);
void *cafvar_getvalue (char *name);
void *cafvar_setvalue (char *name, char *value);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_VAR_H */
/* caf_data_var.h ends here */
