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
#ifndef CAF_DATA_PACKER_H
#define CAF_DATA_PACKER_H 1
/**
 * @defgroup      caf_data_packer           Data Packer
 * @ingroup       caf_data_string
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

#include <caf/caf.h>
#include <caf/caf_data_mem.h>
#include <caf/caf_data_buffer.h>
#include <caf/caf_data_lstdl.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
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
 * Packet unit structure, defines an unit value for a packet
 * definition.
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
 * @brief		Allocates a new unit definition.
 *
 * Allocates a new unit definition, with the given identifier (id),
 * using the known unit type (type), assuming the unit length (length)
 * in the string cases, or using buffer pattern startup (u_start) and
 * the buffer pattern ending (u_end) with starting pattern size (su_sz)
 * and ending pattern size (eu_sz).
 *
 * @param id			unit identifier.
 * @param type			unit type.
 * @param length		unit length.
 * @param u_start		unit startup pattern.
 * @param u_end			unit ending pattern.
 * @param su_sz			unit startup pattern size.
 * @param eu_sz			unit ending pattern size.
 *
 * @return				A new allocated unit.
 */
caf_unit_t *caf_unit_new(int id, caf_unit_type_t type, size_t length,
						 void *u_start, void *u_end, size_t su_sz,
						 size_t eu_sz);

/**
 * @brief		Deallocates the given unit.
 *
 * Deallocates the given unit, this interface does not releases the
 * memory for given startup patterns and ending patterns.
 *
 * @param r				unit to deallocate.
 *
 * @return				CAF_OK on success, CAF_ERROR on failure.
 */
int caf_unit_delete(caf_unit_t *r);

/**
 * @brief		Allocates memory to store the value.
 *
 * Allocates memory for the given unit type, with the given size and
 * the data pointer.
 *
 * @param type			unit type.
 * @param sz			data size.
 * @param data			data pointer.
 *
 * @return		A new allocated value pointer.
 */
caf_unit_value_t *caf_unit_value_new(caf_unit_type_t type, size_t sz,
									 void *data);

/**
 * @brief		Deallocates the memory of a given unit value.
 *
 * Deallocates the memory for the given unit value and data pointer
 * in it.
 *
 * @param r				unit value pointer.
 *
 * @return		CAF_OK on success, CAF_ERROR on failure.
 */
int caf_unit_value_delete(caf_unit_value_t *r);

/**
 * @brief		Default unit deletion callback.
 *
 * More complex units could need another deletion callback. By default
 * this interface is called, but you can specify another callback with
 * the same prototype.
 *
 * @param r				unit to deallocate.
 *
 * @return		CAF_OK on success, CAF_ERROR on failure.
 */
int caf_unit_delete_callback(void *r);

/**
 * @brief		Allocates a new pack definition.
 *
 * Allocates a new pack detinition, creating a new list of units in it
 * and assigning the given identifier (id) to the pack, also you must
 * provide a pack name that can be used for debugging purposes.
 *
 * @param id			pack id.
 * @param name			pack name.
 *
 * @return		A new allocated pack pointer.
 */
caf_pack_t *caf_pack_new(int id, const char *name);

/**
 * @brief		Deallocates the memory used by the given pack.
 *
 * Deallocates the memory used by the given pack. Also deallocates
 * the pack unit list and the name of the given unit.
 *
 * @param r				unit to deallocate.
 *
 * @return		CAF_OK on success, CAF_ERROR on failure.
 */
int caf_pack_delete(caf_pack_t *r);

/**
 * @brief		Allocates memory for a new packet.
 *
 * A packet is a pack definition storage type. A packet stores real
 * data and the pack definition to parse the packet data. Also, need
 * the packet identifier (id), the pack identifier (pack_id) and the
 * pack name (pack_name).
 *
 * @param id			packet id.
 * @param pack_id		pack id.
 * @param pack_name		pack name.
 *
 * @return		A new allocated packet pointer.
 */
caf_packet_t *caf_packet_new(int id, int pack_id, const char *pack_name);

/**
 * @brief		Deallocates the memory of the given packet.
 *
 * Deallocates the memory used by the given packet, also deallocates
 * the pack contained in the packet structure, this means the use of
 * caf_pack_delete.
 *
 * @param r				packet to deallocate.
 *
 * @return		CAF_OK on success, CAF_ERROR on failure.
 */
int caf_packet_delete(caf_packet_t *r);

/**
 * @brief		Adds a new unit to the given packet.
 *
 * Adds a new unit to the given packet, the packet must be previously
 * allocated and need the following parameters: a packet (r), an unit
 * identifier (id), the unit type (type) and the unit length (length).
 * Adding/Creating units does not means that you pass values to the
 * packet, the packet is disassembler from a buffer using parse and
 * translate interfaces.
 *
 * @param r				packet where to add the new unit.
 * @param id			unit id.
 * @param type			unit type.
 * @param length		unit length.
 *
 * @return		CAF_OK on success, CAF_ERROR on failure.
 */
int caf_packet_addunit(caf_packet_t *r, int id, caf_unit_type_t type,
					   size_t length);

/**
 * @brief		Adds a string unit to the given packet.
 *
 * Adds a string unit to the given packet, using or not the string
 * delimitiers or the string size, if the string isn't a delimitied
 * string. The parameters that this interface needs are the packet
 * itself (r), the unit identifier (id), the string length (length),
 * string startup delimitier (u_start), the string end delimitier
 * (u_end) and both delimitier sizes su_sz and eu_sz respectivelly.
 *
 * @param r				packet to add the string unit.
 * @param id			unit identifier.
 * @param length		unit length.
 * @param u_start		string start pattern.
 * @param u_end			string end pattern.
 * @param su_sz			string start pattern size.
 * @param eu_sz			string end pattern size.
 *
 * @return		CAF_OK on success, CAF_ERROR on failure.
 */
int caf_packet_addunitstr(caf_packet_t *r, int id, size_t length,
						  void *u_start, void *u_end, size_t su_sz,
						  size_t eu_sz);

/**
 * @brief		Adds a pascal string unit to the given packet.
 *
 * A pascal string is made from a string length unit, that can
 * be an integer or a byte. The size of the integer value that
 * holds the string length and the string itself as a continuation
 * of the string memory block conforms the entire buffer, then
 * in this interface you specify only the size of the pascal
 * string length holder. The rest of the buffer (usually a void
 * pointer) is taken as the string itself. This interface needs
 * the followin parameters: the packet (r), the unit identifier
 * (id), the string size holder length (length) and -- if the
 * string has a startup pattern -- the startup pattern pointer.
 *
 * @param r				the packet where to add the unit.
 * @param id			the unit identifier
 * @param length		string size holder length (or size).
 * @param u_start		string startup pattern
 *
 * @return		CAF_OK on success, CAF_ERROR on failure.
 */
int caf_packet_addunitpstr(caf_packet_t *r, int id, size_t length,
						   void *u_start);

/**
 * @brief		Gets a string value from the given buffer.
 *
 * Returns the string unit value from the given buffer, assuming
 * that the unit holds a string and the buffer contains enough data
 * to satisfy the pascal string unit definition. The parameters are
 * as follow: the unit definition (u), the buffer where to search
 * for the string (b) and the buffer size (p).
 *
 * @param u				unit definition.
 * @param b				input buffer.
 * @param p				input buffer size.
 *
 * @return		A new allocated unit value, NULL on failure.
 */
caf_unit_value_t *caf_packet_getpstr(caf_unit_t *u, void *b, size_t p);

/**
 * @brief		Gets a string value from the given buffer.
 *
 * Returns the string unit value from the given buffer, assuming
 * that the unit holds a string and the buffer contains enough data
 * to satisfy the unit definition. The parameters are as follow:
 * the unit definition (u), the buffer where to search for the string
 * (b) and the buffer size (p).
 *
 * @param u				unit definition.
 * @param b				input buffer.
 * @param p				input buffer size.
 *
 * @return		A new allocated unit value, NULL on failure.
 */
caf_unit_value_t *caf_packet_getstr(caf_unit_t *u, void *b, size_t p);

/**
 * @brief		Parses a packet from the given input buffer.
 *
 * Parses a packet from the input given buffer. A packet is defined
 * by unit definition, and return the unit definition values in the
 * given packet. Itself, this interface isn't thread safe, but, if
 * you define a pack separatelly from the packet definition, and
 * assing that pack to the packet definition, you can make a thread
 * safe packet.
 *
 * @param r				the packet to parse.
 * @param buf			the input buffer.
 *
 * @return		CAF_OK on success, CAF_ERROR on failure.
 */
int caf_packet_parse(caf_packet_t *r, cbuffer_t *buf);

/**
 * @brief		Translates the given packet into a buffer.
 *
 * Translates the given buffer into a packet, ready to be sent
 * over the network or write a file. This can be usefull with
 * strong varaying packets protocolos and files.
 *
 * @param r				the packet to translate.
 *
 * @return		The allocated buffer with translated data, NULL on
 *				failure.
 */
cbuffer_t *caf_packet_translate(caf_packet_t *r);


#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_PACKER_H */
/* caf_data_packer.h ends here */

