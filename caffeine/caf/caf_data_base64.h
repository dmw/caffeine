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


cbuffer_t *caf_base64_encode (cbuffer_t *inb, cbuffer_t *padb,
							  int padc);
cbuffer_t *caf_base64_decode (cbuffer_t *inb);

cbuffer_t *caf_base64_encode_stream (cbuffer_t *inb, size_t spos,
									 size_t ssz, cbuffer_t *padb,
									 int padc);
cbuffer_t *caf_base64_decode_stream (cbuffer_t *inb, size_t ssz);

caf_io_file_t *caf_base64_encode_file (caf_io_file_t *outf,
									   caf_io_file_t *inf,
									   cbuffer_t *padb, int padc);

caf_io_file_t *caf_base64_decode_file (caf_io_file_t *outf,
									   caf_io_file_t *inf);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_BASE64_H */
/* caf_data_base64.h ends here */
