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

    $Id: caf_data_pcre.h 198 2007-02-15 10:13:36Z dmw $
 */
#ifndef CAF_REGEX_PCRE_H
#define CAF_REGEX_PCRE_H 1

#include <pcre.h>

#include <caf/caf_data_lstdl.h>

/**
 * @defgroup      caf_regex_pcre        PCRE Wrapper
 * @ingroup       caf_data_string
 * @addtogroup    caf_regex_pcre
 * @{
 *
 * @brief     Caffeine PCRE Wrapper.
 * @date      $Date: 2007-02-15 07:13:36 -0300 (Thu, 15 Feb 2007) $
 * @version   $Revision: 198 $
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine PCRE Wrapper.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#define REGEX_PCRE_T_SZ             (sizeof (regex_pcre_t))
#define REGEX_PCRE_POOL_SZ          (sizeof (regex_pcre_pool_t))
#define REGEX_PCRE_OV_SZ            128

typedef struct regex_pcre_s regex_pcre_t;
struct regex_pcre_s {
    int id;
    int opt;
    char *pattern;
    pcre *code;
    pcre_extra *study;
    int erroff;
    char *errstr;
};

typedef struct regex_pcre_pool_s regex_pcre_pool_t;
struct regex_pcre_pool_s {
    int id;
    char *name;
    lstdl_t *pool;
};

regex_pcre_t *regex_pcre_new (const int id, const char *pat, const int opt,
                              const unsigned char *tbl, const int study);
int regex_pcre_delete (regex_pcre_t *re);
int regex_pcre_match (regex_pcre_t *re, const char *sub);
int regex_pcre_find (regex_pcre_t *re, const char *sub, int *ov);
int regex_pcre_subs (regex_pcre_t *re, const char *sub, cbuffer_t *out);
regex_pcre_pool_t *regex_pcre_pool_new (const int id, const char *name);
int regex_pcre_pool_delete (regex_pcre_pool_t *p);
int regex_pcre_pool_match (regex_pcre_pool_t *p, const char *sub);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_REGEX_PCRE_H */
/* caf_regex_pcre.h ends here */
