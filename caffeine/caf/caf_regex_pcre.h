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
#ifndef CAF_REGEX_PCRE_H
#define CAF_REGEX_PCRE_H 1
/**
 * @defgroup      caf_refex_pcre    PCRE Regular Expressions
 * @ingroup       caf_refex
 * @addtogroup    caf_refex_pcre
 * @{
 *
 * @brief     PCRE Regular Expressions
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * PCRE Regular Expressions.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

typedef struct regex_pcre_s regex_pcre_t;
struct regex_pcre_s {
	int id;
	int opt;
	pcre *code;
	pcre_extra *study;
	char *pattern;
	int erroff;
	char *errstr;
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
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_REGEX_PCRE_H */
/* caf_regex_pcre.h ends here */
