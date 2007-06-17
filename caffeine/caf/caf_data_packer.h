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
#ifndef CAF_DATA_PACKER_H
#define CAF_DATA_PACKER_H 1
/**
 * @defgroup      caf_data_packer           Data Packer
 * @ingroup       caf_data
 * @addtogroup    caf_data_packer
 * @{
 *
 * @brief     Data Packer
 * @date      $Date$
 * @version   $Revision$
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

/** Packer unit structure size */
#define CAF_UNIT_SZ                 (sizeof (caf_unit_t))
/** Packer pack structure size */
#define CAF_PACK_SZ                 (sizeof (caf_pack_t))
/** Packer packer structure size */
#define CAF_PACKET_SZ               (sizeof (caf_packet_t))
/** Packer unit value structure size */
#define CAF_UNIT_VALUE_SZ           (sizeof (caf_unit_value_t))

/** OCTET type */
typedef u_int8_t caf_unit_octet_t;
/** WORD (x86 based) type */
typedef u_int16_t caf_unit_word_t;
/** DWORD (x86 based) type */
typedef u_int32_t caf_unit_dword_t;
/** QWORD (x86 based) type */
typedef u_int64_t caf_unit_qword_t;

/** OCTET size */
#define CAF_UNIT_OCTET_SZ           (sizeof (caf_unit_octet_t))
/** WORD size */
#define CAF_UNIT_WORD_SZ            (sizeof (caf_unit_word_t))
/** DWORD size */
#define CAF_UNIT_DWORD_SZ           (sizeof (caf_unit_dword_t))
/** QWORD size */
#define CAF_UNIT_QWORD_SZ           (sizeof (caf_unit_qword_t))


/**
 *
 * @brief    Defines the types that the packer can handle
 * Defines the types that the packer can handle to process the data
 * in packet definitions.
 */
typedef enum {
    /** OCTET type */
    CAF_UNIT_OCTET,
    /** WORD type */
    CAF_UNIT_WORD,
    /** DWORD type */
    CAF_UNIT_DWORD,
    /** QWORD type */
    CAF_UNIT_QWORD,
    /** STRING type */
    CAF_UNIT_STRING,
    /** PASCAL STRING type */
    CAF_UNIT_PSTRING
} caf_unit_type_t;


/**
 *
 * @brief    Pack Unit Type
 * The type of a pack unit
 * @see      caf_unit_s
 */
typedef struct caf_unit_s caf_unit_t;
/**
 *
 * @brief    Pack Unit Structure
 * Pack unit structure, defines an unit for a packet definition.
 * @see      caf_unit_t
 */
struct caf_unit_s {
    /** Unit ID */
    int id;
    /** Unit Type (@see caf_unit_type_t) */
    int type;
    /** Unit fixed length */
    size_t length;
    /** Unit start pattern data pointer (for strings) */
    void *u_start;
    /** Unit end pattern data pointer (for strings) */
    void *u_end;
    /** Unit start pattern data size (for strings) */
    size_t su_sz;
    /** Unit end pattern data size (for strings) */
    size_t eu_sz;
};


/**
 *
 * @brief    Packet Unit Value Type
 * The type of a packet unit value.
 * @see      caf_unit_value_s
 */
typedef struct caf_unit_value_s caf_unit_value_t;
/**
 *
 * @brief    Packet Unit Structure
 * Packet unit structure, defines an unit value for a packet definition.
 * @see      caf_unit_value_t
 */
struct caf_unit_value_s {
    /** Unit Type (@see caf_unit_type_t) */
    int type;
    /** Unit fixed size */
    size_t sz;
    /** Unit data */
    void *data;
};


/**
 *
 * @brief    Packet Definition Type
 * Packet definition type
 * @see      caf_pack_s
 */
typedef struct caf_pack_s caf_pack_t;
/**
 *
 * @brief    Packet Definition Structure
 * Packet definition structure, defines a packet with a limited number of
 * units, and gives you a dynamic packet definitions that should change
 * in runtime.
 * @see      caf_pack_t
 */
struct caf_pack_s {
    /** Pack/Packet Id */
    int id;
    /** Pack/Packet Name */
    char *name;
    /** Units conforming the packet (do not add units directly) */
    lstdl_t *units;
};


/**
 *
 * @brief    Data Packet Type
 * Data Packet type
 * @see      caf_packet_s
 */
typedef struct caf_packet_s caf_packet_t;
/**
 *
 * @brief    Data Packet Structure
 * Data packet structure contains parsed packets from a given buffer and
 * a packet contaings parsed units.
 * @see      caf_packet_t
 */
struct caf_packet_s {
    /** Packet Id */
    int id;
    /** Packet Definition */
    caf_pack_t *pack;
    /** Parsed Packets */
    lstdl_t *packets;
};


/**
 *
 * @brief    Creates a new Unit.
 *
 * Creates a new Unit definition, you can define units in a pack (packet
 * definition) to use with the automated unpacking functions.
 *
 * @param[in]    id         Unit Id.
 * @param[in]    type       Unit Type (@see caf_unit_type_t).
 * @param[in]    length     the unit length (to use with strings).
 * @param[in]    u_start    unit pattern startup byte string.
 * @param[in]    u_end      unit pattern ending byte string.
 * @param[in]    su_sz      unit pattern startup byte string size.
 * @param[in]    eu_sz      unit pattern ending byte string.
 * @return       caf_unit_t *     the allocated unit.
 *
 * @see      caf_unit_type_t
 * @see      caf_unit_t
 * @see      caf_pack_t
 * @see      caf_packet_t
 */
caf_unit_t *caf_unit_new (int id, caf_unit_type_t type, size_t length,
                          void *u_start, void *u_end, size_t su_sz,
                          size_t eu_sz);


/**
 *
 * @brief    Deletes (deallocates) the given unit.
 *
 * Deletes or deallocates the memory of the given unit pointer.
 *
 * @param[in]    r          The unit to deallocate.
 * @return       int        CAF_OK on success, CAF_ERROR on failure.
 *
 * @see      caf_unit_t
 */
int caf_unit_delete (caf_unit_t *r);


/**
 *
 * @brief    Creates a new Unit Value.
 *
 * Allocates (creates) a new unit value, assigning the corresponding unit data.
 *
 * @param[in]    type       Unit value type.
 * @param[in]    sz         Unit value size.
 * @param[in]    data       Unit value data.
 * @return       caf_unit_value_t *         The allocated unit value.
 *
 * @see      caf_unit_type_t
 * @see      caf_unit_t
 * @see      caf_unit_value_t
*/
caf_unit_value_t *caf_unit_value_new (caf_unit_type_t type, size_t sz,
                                      void *data);


/**
 *
 * @brief    Deletes (deallocates) the given unit value.
 *
 * Deletes or deallocates the memory of the given unit value pointer.
 *
 * @param[in]    r          The unit value to deallocate.
 * @return       int        CAF_OK on success, CAF_ERROR on failure.
 *
 * @see      caf_unit_value_t
 */
int caf_unit_value_delete (caf_unit_value_t *r);


/**
 *
 * @brief    Delete callback to use with pack (packet definition) deletion
 *
 * Deletes the given unit definition.
 *
 * @param[in]    r          The unit to deallocate.
 * @return       int        CAF_OK on success, CAF_ERROR on failure.
 *
 * @see      caf_unit_t
 */
int caf_unit_delete_callback (void *r);


/**
 *
 * @brief    Creates a new empty Pack (packet definition).
 *
 * Creates (allocates) a new empty Pack (packet definition) to use with
 * the pack (packet definition) functions.
 *
 * @param[in]    id         Pack Id
 * @param[in]    name       Pack Name
 * @return       caf_pack_t *       a new allocated empty pack.
 *
 * @see      caf_pack_t
*/
caf_pack_t *caf_pack_new (int id, const char *name);


/**
 *
 * @brief    Deletes the given pack (packet definition)
 *
 * Deletes the given pack or packet definition, using the unit delete
 * callback to immediately
 *
 * @param[in]    r          Pack to delete
 * @return       int        CAF_OK on success, CAF_ERROR on failure.
 *
 * @see      caf_pack_t
 */
int caf_pack_delete (caf_pack_t *r);


/**
 *
 * @brief    Creates a new empty Packet.
 *
 * Allocates memory (creates) a new empty packet.
 *
 * @param[in]    id         Pack Id
 * @param[in]    name       Pack Name
 * @return       caf_pack_t *       a new allocated empty pack.
 *
 * @see      caf_pack_t
 */
caf_packet_t *caf_packet_new (int id, int pack_id, const char *pack_name);


/**
 *
 * @brief    Deletes the given Packet
 *
 * Deallocates the memory for the given packet, and deletes the given units.
 *
 * @param[in]    id         Pack Id
 * @param[in]    name       Pack Name
 * @return       caf_pack_t *       a new allocated empty pack.
 *
 * @see      caf_pack_t
 */
int caf_packet_delete (caf_packet_t *r);


/**
 *
 * @brief    Adds an Unit to the give pack
 *
 * Deallocates the memory for the given packet, and deletes the given units.
 *
 * @param[in]    id         Pack Id
 * @param[in]    name       Pack Name
 * @return       caf_pack_t *       a new allocated empty pack.
 *
 * @see      caf_pack_t
 */
int caf_packet_addunit (caf_packet_t *r, int id, caf_unit_type_t type,
                        size_t length);

int caf_packet_addunitstr (caf_packet_t *r, int id, size_t length,
                           void *u_start, void *u_end, size_t su_sz,
                           size_t eu_sz);

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
