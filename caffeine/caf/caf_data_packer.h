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

    $Id: caf_data_packer.h 199 2007-02-17 21:03:28Z dmw $
 */
#ifndef CAF_DATA_PACKER_H
#define CAF_DATA_PACKER_H 1
/**
 * @defgroup      caf_data_packer           Data Packer
 * @ingroup       caf_data
 * @addtogroup    caf_data_packer
 * @{
 *
 * @brief     Data Packer
 * @date      $Date: 2007-02-17 18:03:28 -0300 (Sat, 17 Feb 2007) $
 * @version   $Revision: 199 $
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Data Packer
 *
 */

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_data_lstdl.h"

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#define CAF_UNIT_SZ                 (sizeof (caf_unit_t))
#define CAF_PACK_SZ                 (sizeof (caf_pack_t))
#define CAF_PACKET_SZ               (sizeof (caf_packet_t))
#define CAF_UNIT_VALUE_SZ           (sizeof (caf_unit_value_t))

typedef u_int8_t caf_unit_octet_t;
typedef u_int16_t caf_unit_word_t;
typedef u_int32_t caf_unit_dword_t;
typedef u_int64_t caf_unit_qword_t;

#define CAF_UNIT_OCTET_SZ           (sizeof (caf_unit_octet_t))
#define CAF_UNIT_WORD_SZ            (sizeof (caf_unit_word_t))
#define CAF_UNIT_DWORD_SZ           (sizeof (caf_unit_dword_t))
#define CAF_UNIT_QWORD_SZ           (sizeof (caf_unit_qword_t))

typedef enum {
    CAF_UNIT_OCTET,
    CAF_UNIT_WORD,
    CAF_UNIT_DWORD,
    CAF_UNIT_QWORD,
    CAF_UNIT_STRING,
    CAF_UNIT_PSTRING
} caf_unit_type_t;

typedef struct caf_unit_s caf_unit_t;
struct caf_unit_s {
    int id;
    int type;
    size_t length;
    void *u_start;
    void *u_end;
    size_t su_sz;
    size_t eu_sz;
};

typedef struct caf_unit_value_s caf_unit_value_t;
struct caf_unit_value_s {
    int type;
    size_t sz;
    void *data;
};

typedef struct caf_pack_s caf_pack_t;
struct caf_pack_s {
    int id;
    char *name;
    lstdl_t *units;
};

typedef struct caf_packet_s caf_packet_t;
struct caf_packet_s {
    int id;
    caf_pack_t *pack;
    lstdl_t *packets;
};


caf_unit_t *caf_unit_new (int id, caf_unit_type_t type, size_t length,
                          void *u_start, void *u_end, size_t su_sz,
                          size_t eu_sz);
int caf_unit_delete (caf_unit_t *r);
caf_unit_value_t *caf_unit_value_new (caf_unit_type_t type, size_t sz,
                                      void *data);
int caf_unit_value_delete (caf_unit_value_t *r);
int caf_unit_delete_callback (void *r);
caf_pack_t *caf_pack_new (int id, const char *name);
int caf_pack_delete (caf_pack_t *r);
caf_packet_t *caf_packet_new (int id, int pack_id, const char *pack_name);
int caf_packet_delete (caf_packet_t *r);
int caf_packet_addunit (caf_packet_t *r, int id, caf_unit_type_t type,
                        size_t length);
int caf_packet_addstr (caf_packet_t *r, int id, size_t length, void *u_start,
                       void *u_end, size_t su_sz, size_t eu_sz);
int caf_packet_parse (caf_packet_t *r, cbuffer_t *buf);
caf_unit_value_t *caf_packet_getstr (caf_unit_t *u, void *b, size_t p);
caf_unit_value_t *caf_packet_getpascalstr (caf_unit_t *u, void *b, size_t p);
cbuffer_t *caf_packet_translate (caf_packet_t *r);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_PACKER_H */
/* caf_data_packer.h ends here */
