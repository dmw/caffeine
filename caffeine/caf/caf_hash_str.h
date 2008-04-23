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
#ifndef CAF_HASH_STR_H
#define CAF_HASH_STR_H 1
/**
 * @defgroup      caf_hash_str    String Hash Functions
 * @ingroup       caf_data_string
 * @addtogroup    caf_hash_str
 * @{
 *
 * @brief     String Hash Functions.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@coder.cl>
 *
 * String Hash Functions
 *
 */

#include <sys/types.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Common string hashing function (interface) define */
#define CAF_HASH_STR_FUNCTION(f)    \
    u_int32_t (*f) (const char *str, const u_int32_t len)

/**
 * Computes the RS Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_rs (const char *str, const u_int32_t len);

/**
 * Computes the JS Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_js (const char *str, const u_int32_t len);

/**
 * Computes the PWJ Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_pjw (const char *str, const u_int32_t len);

/**
 * Computes the ELF Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_elf (const char *str, const u_int32_t len);

/**
 * Computes the BKDR Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_bkdr (const char *str, const u_int32_t len);

/**
 * Computes the SDBM Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_sdbm (const char *str, const u_int32_t len);

/**
 * Computes the DJB Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_djb (const char *str, const u_int32_t len);

/**
 * Computes the RS Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_dek (const char *str, const u_int32_t len);

/**
 * Computes the BP Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_bp (const char *str, const u_int32_t len);

/**
 * Computes the FNV Hash for the given string (str) with
 * the given length (len).
 *
 * @param str			input string
 * @param len			string length
 *
 * @return Computed hash.
 */
u_int32_t caf_shash_fnv (const char *str, const u_int32_t len);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_HASH_STR_H */
/* caf_hash_str.h ends here */

