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
#ifndef CAF_DATA_BASE64_H
#define CAF_DATA_BASE64_H 1
/**
 * @defgroup      caf_encoding_base64    Base 64 Encoding/Decoding
 * @ingroup       caf_encoding
 * @addtogroup    caf_encoding_base64
 * @{
 *
 * @brief     Base 64 Encoding/Decoding.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Base 64 Encoding functions.
 */
#include <caf/caf_tool_macro.h>
#include <caf/caf_data_buffer.h>
#include <caf/caf_io_file.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */


/**
 * @brief		Encode to Base64 the given buffer.
 *
 * Encodes the given data buffer into Base 64 encoding returning
 * null on error. The interface makes only one allocation of memory,
 * be carefull encoding large buffers. For large files and network
 * connections use the proper function. The padding buffer and the
 * padding column are optional parameters.
 *
 * @param[in]	inb				the input buffer.
 * @param[in]	padb			the padding buffer.
 * @param[in]	padc			the padding column.
 *
 * @see			cbuffer_t
 * @see			caf_base64_decode
 */
cbuffer_t *caf_base64_encode (cbuffer_t *inb, cbuffer_t *padb,
							  int padc);

/**
 * @brief		Decodes from Base 64 the given buffer.
 *
 * Decodes the given data buffer from Base 64 encoding returning null
 * on error. The interface makes only two allocation of memory, be
 * carefull decoding large buffers. For large files and network
 * connections use the proper function.
 *
 * @param[in]	inb				the input buffer.
 *
 * @see			cbuffer_t
 * @see			caf_base64_encode
 */
cbuffer_t *caf_base64_decode (cbuffer_t *inb);

/**
 * @brief		Encode to Base 64 streaming data from buffers.
 *
 * Encodes to Base 64 the given buffer assuming that the given
 * buffer contains streaming data. The total size of the streaming
 * data size must be known by the system, also the current postion
 * on the streaming data must be known too. The padding buffer and
 * padding column are optional parameters.
 *
 * @param[in]	inb				the input buffer.
 *
 * @see			cbuffer_t
 * @see			caf_base64_decode_stream
 */
cbuffer_t *caf_base64_encode_stream (cbuffer_t *inb, size_t spos,
									 size_t ssz, cbuffer_t *padb,
									 int padc);

/**
 * @brief		Decodes from Base 64 streaming data to buffers.
 *
 * Decodes from Base 64 data the given buffer assuming that the given
 * buffer contains streaming data. You need cache buffer to store
 * residual data from the streaming buffer. The residual data contains
 * the slices that can't be converted, for future use in the next
 * call to this interface.
 *
 * @param[in]	inb				the input buffer.
 * @param[in]	cres			residual cache buffer.
 *
 * @see			cbuffer_t
 * @see			caf_base64_encode_stream
 */
cbuffer_t *caf_base64_decode_stream (cbuffer_t *inb, cbuffer_t *cres);

/**
 * @brief		Encodes the given file into Base 64.
 *
 * This interface uses the caf_base64_encode_stream to encode files.
 * This is donde using slices of the file, asuming that the file can
 * be huge and can't be memory mapped.
 *
 * @param[in]	inb				the input buffer.
 * @param[in]	cres			residual cache buffer.
 *
 * @see			cbuffer_t
 * @see			caf_base64_encode_stream
 * @see			caf_base64_decode_stream
 * @see			caf_base64_decode_file
 */
caf_io_file_t *caf_base64_encode_file (caf_io_file_t *outf,
									   caf_io_file_t *inf,
									   cbuffer_t *padb, int padc);

/**
 * @brief		Decodes the given file from Base 64.
 *
 * This interface uses the caf_base64_decode_stream to decode files.
 * This is donde in using slices of the file, asuming that the file can
 * be huge and can't be memory mapped.
 *
 * @param[in]	inb				the input buffer.
 * @param[in]	cres			residual cache buffer.
 *
 * @see			cbuffer_t
 * @see			caf_base64_encode_stream
 * @see			caf_base64_decode_stream
 * @see			caf_base64_encode_file
 */
caf_io_file_t *caf_base64_decode_file (caf_io_file_t *outf,
									   caf_io_file_t *inf);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_BASE64_H */
/* caf_data_base64.h ends here */
