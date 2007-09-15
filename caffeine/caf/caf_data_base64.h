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

#include <caf/caf_io_file.h>

#ifdef __cplusplus
CAF_START_C_EXTERNS
#endif /* !__cplusplus */

#ifndef COMPILING_CAFFEINE
extern const char *caf_base16_alphabet;
extern const char *caf_base32_alphabet;
extern const char *caf_base64_alphabet;
extern const char *caf_base64_alphabet_url;
#endif

size_t base_encode_chunk_sz(int bits);

size_t base_decode_chunk_sz(int bits);

size_t base_encode_buffer_sz(int bits);

size_t base_decode_buffer_sz(int bits);

cbuffer_t *caf_base16_encode(cbuffer_t *in);

cbuffer_t *caf_base16_decode(cbuffer_t *in);

cbuffer_t *caf_base32_encode(cbuffer_t *in);

cbuffer_t *caf_base32_decode(cbuffer_t *in);

cbuffer_t *caf_base64_encode(cbuffer_t *in);

cbuffer_t *caf_base64_decode(cbuffer_t *in);

cbuffer_t *caf_base64_encode_url(cbuffer_t *in);

cbuffer_t *caf_base64_decode_url(cbuffer_t *in);

caf_io_file_t *caf_base16_encode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

caf_io_file_t *caf_base16_decode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

caf_io_file_t *caf_base32_encode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

caf_io_file_t *caf_base32_decode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

caf_io_file_t *caf_base64_encode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

caf_io_file_t *caf_base64_decode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

cbuffer_t *caf_base_encode(cbuffer_t *buf, const char *codes,
						   const int bits, size_t qn);

cbuffer_t *caf_base_decode(cbuffer_t *buf, const char *codes,
						   const int bits);

cbuffer_t *caf_base_encode_stream(cbuffer_t *buf, const char *codes,
								  const int bits, size_t qn,
								  cbuffer_t *cache);

cbuffer_t *caf_base_decode_stream(cbuffer_t *buf, const char *codes,
								  const int bits, cbuffer_t *cache);

caf_io_file_t *caf_base_encode_file(caf_io_file_t *inf, caf_io_file_t *outf,
									const char *alpha, int bits, size_t qn);

caf_io_file_t *caf_base_decode_file(caf_io_file_t *inf, caf_io_file_t *outf,
									const char *alpha, int bits);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_BASE64_H */
/* caf_data_base64.h ends here */
