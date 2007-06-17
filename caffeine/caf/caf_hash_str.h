/* -*- mode: c; c-default-style: "k&r"; indent-tabs-mode: nil; -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 expandtab: */
/* kate: space-indent on; indent-width 4; mixedindent off; indent-mode none; */
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
#ifndef CAF_HASH_STR_H
#define CAF_HASH_STR_H 1
/**
 * @defgroup      caf_hash_str    String Hash Functions
 * @ingroup       caf_mem
 * @addtogroup    caf_hash_str
 * @{
 *
 * @brief     String Hash Functions.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * String Hash Functions
 *
 */

#include <sys/types.h>

u_int32_t caf_shash_rs (const char *str, u_int32_t len);
u_int32_t caf_shash_js (const char *str, u_int32_t len);
u_int32_t caf_shash_pjw (const char *str, u_int32_t len);
u_int32_t caf_shash_elf (const char *str, u_int32_t len);
u_int32_t caf_shash_bkdr (const char *str, u_int32_t len);
u_int32_t caf_shash_sdbm (const char *str, u_int32_t len);
u_int32_t caf_shash_djb (const char *str, u_int32_t len);
u_int32_t caf_shash_dek (const char *str, u_int32_t len);
u_int32_t caf_shash_bp (const char *str, u_int32_t len);
u_int32_t caf_shash_fnv (const char *str, u_int32_t len);

/** }@ */
#endif /* !CAF_HASH_STR_H */
/* caf_hash_str.h ends here */
