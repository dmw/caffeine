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
#ifndef CAF_DATA_BASE64_H
#define CAF_DATA_BASE64_H 1

#include <caf/caf_io_file.h>

/**
 * @defgroup      caf_data_base64    Base Encoding Interfaces
 * @ingroup       caf_data_string
 * @addtogroup    caf_data_base64
 * @{
 *
 * @brief     Caffeine Base Encoding Interfaces
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine Base Encoding interfaces, provides common interfaces for
 * base encoding operations. You can encode/decode using known encodings
 * such as Base16, Base32 and Base64. Also, provides interfaces to
 * implement propertary encodings.
 *
 */
#ifdef __cplusplus
CAF_START_C_EXTERNS
#endif /* !__cplusplus */

#ifndef COMPILING_CAFFEINE
extern const char *caf_base16_alphabet;
extern const char *caf_base32_alphabet;
extern const char *caf_base64_alphabet;
extern const char *caf_base64_alphabet_url;
#endif

/**
 * @brief		Encoding Chunk Size.
 *
 * Returns the encoding chunk size for the given bits, ie. for
 * base 64 encoding 6 bits are passed as argument, and returns
 * 4.
 *
 * @param bits					encoding bits.
 *
 * @return Chunk size of encoding operation.
 */
size_t base_encode_chunk_sz(int bits);

/**
 * @brief		Decoding Chunk Size.
 *
 * Returns the decoding chunk size for the given bits, ie. for
 * base 64 encoding 6 bits are passed as argument, and returns
 * 3.
 *
 * @param bits					encoding bits.
 *
 * @return Chunk size of decoding operation.
 */
size_t base_decode_chunk_sz(int bits);

/**
 * @brief		Encoding buffer size.
 *
 * Calculates the buffer size for encoding operations, using
 * chunk size as the base of calculations. This function is
 * used by streaming encoding operations.
 *
 * @param bits					encoding bits.
 *
 * @return Encoding buffer size
 */
size_t base_encode_buffer_sz(int bits);

/**
 * @brief		Decoding buffer size.
 *
 * Calculates the buffer size for decoding operations, using
 * chunk size as the base of calculations. This function is
 * used by streaming decoding operations.
 *
 * @param bits					encoding bits.
 *
 * @return Decoding buffer size
 */
size_t base_decode_buffer_sz(int bits);

/**
 * @brief		Buffer base 16 encoding.
 *
 * Encodes small buffers with base 16 encoding. Is not recommended
 * to be used in large files or streaming buffers, instead use the
 * streaming interfaces.
 *
 * @param in					the input buffer.
 *
 * @return Encoded buffer.
 */
cbuffer_t *caf_base16_encode(cbuffer_t *in);

/**
 * @brief		Buffer base 16 decoding.
 *
 * Decodes small buffers with base 16 encoding. Is not recommended
 * to be used in large files or streaming buffers, instead use the
 * streaming interfaces.
 *
 * @param in					the input buffer.
 *
 * @return Decoded buffer.
 */
cbuffer_t *caf_base16_decode(cbuffer_t *in);

/**
 * @brief		Buffer base 32 encoding.
 *
 * Encodes small buffers with base 32 encoding. Is not recommended
 * to be used in large files or streaming buffers, instead use the
 * streaming interfaces.
 *
 * @param in					the input buffer.
 *
 * @return Encoded buffer.
 */
cbuffer_t *caf_base32_encode(cbuffer_t *in);

/**
 * @brief		Buffer base 32 decoding.
 *
 * Decodes small buffers with base 32 encoding. Is not recommended
 * to be used in large files or streaming buffers, instead use the
 * streaming interfaces.
 *
 * @param in					the input buffer.
 *
 * @return Decoded buffer.
 */
cbuffer_t *caf_base32_decode(cbuffer_t *in);

/**
 * @brief		Buffer base 64 encoding.
 *
 * Encodes small buffers with base 64 encoding. Is not recommended
 * to be used in large files or streaming buffers, instead use the
 * streaming interfaces.
 *
 * @param in					the input buffer.
 *
 * @return Encoded buffer.
 */
cbuffer_t *caf_base64_encode(cbuffer_t *in);

/**
 * @brief		Buffer base 64 decoding.
 *
 * Decodes small buffers with base 64 encoding. Is not recommended
 * to be used in large files or streaming buffers, instead use the
 * streaming interfaces.
 *
 * @param in					the input buffer.
 *
 * @return Decoded buffer.
 */
cbuffer_t *caf_base64_decode(cbuffer_t *in);

/**
 * @brief		Buffer base 64 URL encoding.
 *
 * Encodes small buffers with base 64 encoding. Is not recommended
 * to be used in large files or streaming buffers, instead use the
 * streaming interfaces. The purpose is to encode URLs, with some
 * replacements in the encoding alphabet as '+' by '-' and '/' by
 * '_'.
 *
 * @param in					the input buffer.
 *
 * @return Encoded buffer.
 */
cbuffer_t *caf_base64_encode_url(cbuffer_t *in);

/**
 * @brief		Buffer base 64 URL decoding.
 *
 * Decodes small buffers with base 64 encoding. Is not recommended
 * to be used in large files or streaming buffers, instead use the
 * streaming interfaces. The purpose is to encode URLs, with some
 * replacements in the encoding alphabet as '+' by '-' and '/' by
 * '_'.
 *
 * @param in					the input buffer.
 *
 * @return Decoded buffer.
 */
cbuffer_t *caf_base64_decode_url(cbuffer_t *in);

/**
 * @brief		File base 16 encoding.
 *
 * Encodes a file in base 16 encoding. The core interfaces used
 * in this task, are the stream encoding capable interfaces and
 * both calculation interfaces.
 *
 * @param inf					input file.
 * @param outf					output file.
 *
 * @return The encoded file, NULL on failure.
 */
caf_io_file_t *caf_base16_encode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

/**
 * @brief		File base 16 decoding.
 *
 * Decodes a file in base 16 encoding. The core interfaces used
 * in this task, are the stream encoding capable interfaces and
 * both calculation interfaces.
 *
 * @param inf					input file.
 * @param outf					output file.
 *
 * @return The encoded file, NULL on failure.
 */
caf_io_file_t *caf_base16_decode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

/**
 * @brief		File base 32 encoding.
 *
 * Encodes a file in base 32 encoding. The core interfaces used
 * in this task, are the stream encoding capable interfaces and
 * both calculation interfaces.
 *
 * @param inf					input file.
 * @param outf					output file.
 *
 * @return The encoded file, NULL on failure.
 */
caf_io_file_t *caf_base32_encode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

/**
 * @brief		File base 32 decoding.
 *
 * Decodes a file in base 32 encoding. The core interfaces used
 * in this task, are the stream encoding capable interfaces and
 * both calculation interfaces.
 *
 * @param inf					input file.
 * @param outf					output file.
 *
 * @return The encoded file, NULL on failure.
 */
caf_io_file_t *caf_base32_decode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

/**
 * @brief		File base 64 encoding.
 *
 * Encodes a file in base 64 encoding. The core interfaces used
 * in this task, are the stream encoding capable interfaces and
 * both calculation interfaces.
 *
 * @param inf					input file.
 * @param outf					output file.
 *
 * @return The encoded file, NULL on failure.
 */
caf_io_file_t *caf_base64_encode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

/**
 * @brief		File base 64 decoding.
 *
 * Decodes a file in base 64 encoding. The core interfaces used
 * in this task, are the stream encoding capable interfaces and
 * both calculation interfaces.
 *
 * @param inf					input file.
 * @param outf					output file.
 *
 * @return The encoded file, NULL on failure.
 */
caf_io_file_t *caf_base64_decode_file(caf_io_file_t *inf,
									  caf_io_file_t *outf);

/**
 * @brief		Core Base Encoding.
 *
 * Core base encoding interface. This interface does all the encoding
 * job. As parameters, need a buffer (buf), an alphabet (codes), the
 * encoding bits (bits) and the encoding quantum (qn). Known bits and
 * encoding alphabets implemented by default are Base 16/32/64 and
 * Base 64 URL alphabets. You can modify any known alphabet for private
 * o propertary encodings.
 *
 * @param buf			input buffer.
 * @param codes			input alphabet.
 * @param bits			input encoding bits.
 * @param qn			encoding quantum.
 *
 * @return A base encoded buffer, NULL on failure.
 */
cbuffer_t *caf_base_encode(cbuffer_t *buf, const char *codes,
						   const int bits, size_t qn);

/**
 * @brief		Core Base Decoding.
 *
 * Core base decoding interface. This interface does all the decoding
 * job. As parameters, need a buffer (buf), an alphabet (codes) and the
 * encoding bits (bits). Known bits and encoding alphabets implemented
 * by default are Base 16/32/64 and Base 64 URL alphabets. You can modify
 * any known alphabet for private o propertary encodings.
 *
 * @param buf			input buffer.
 * @param codes			input alphabet.
 * @param bits			input encoding bits.
 * @param qn			encoding quantum.
 *
 * @return A base encoded buffer, NULL on failure.
 */
cbuffer_t *caf_base_decode(cbuffer_t *buf, const char *codes,
						   const int bits);

/**
 * Core streaming encoding interface. This interface does all the streaming
 * encoding job. As parameters needs the buffer (buf), the encoding alphabet
 * (codes), the encoding bits (bits), the encoding quantum (qn) and the
 * stream cache. The last argument, the encoding cache, is a small buffer,
 * usually with a size less than the encoding chunk size, and stores the
 * remaining bytes of encoding operations.
 *
 * @param buf			input buffer.
 * @param codes			input alphabet.
 * @param bits			encoding bits.
 * @param qn			encoding quantum.
 * @param cache			encoding cache.
 *
 * @return A stream encoded slice, NULL on failure.
 */
cbuffer_t *caf_base_encode_stream(cbuffer_t *buf, const char *codes,
								  const int bits, size_t qn,
								  cbuffer_t *cache);

/**
 * @brief		Core Base Strem Encoding.
 *
 * Core streaming decoding interface. This interface does all the streaming
 * decoding job. As parameters needs the buffer (buf), the encoding alphabet
 * (codes), the encoding bits (bits) and the stream cache. The last argument,
 * the encoding cache, is a small buffer, usually with a size less than the
 * encoding chunk size, and stores the remaining bytes of encoding operations.
 *
 * @param buf			input buffer.
 * @param codes			input alphabet.
 * @param bits			encoding bits.
 * @param qn			encoding quantum.
 * @param cache			encoding cache.
 *
 * @return A stream encoded slice, NULL on failure.
 */
cbuffer_t *caf_base_decode_stream(cbuffer_t *buf, const char *codes,
								  const int bits, cbuffer_t *cache);

/**
 * @brief		Core Base File Decoding.
 *
 * Core file encoding interface. This interface does all the file encoding
 * job. Is based on stream encoding the interface, and does the encoding
 * job by slices, this means that you can safety encode large files, depending
 * on your file system.
 *
 * @param inf			input file.
 * @param outf			output file.
 * @param alpha			encoding alphabet.
 * @param bits			encoding bits.
 * @param qn			encoding quantum.
 *
 * @return A base encoded file, NULL on failure.
 */
caf_io_file_t *caf_base_encode_file(caf_io_file_t *inf, caf_io_file_t *outf,
									const char *alpha, int bits, size_t qn);

/**
 * @brief		Core Base File Decoding.
 *
 * Core file decoding interface. This interface does all the file decoding
 * job. Is based on stream decoding the interface, and does the decoding
 * job by slices, this means that you can safety encode large files, depending
 * on your file system.
 *
 * @param inf			input file.
 * @param outf			output file.
 * @param alpha			encoding alphabet.
 * @param bits			encoding bits.
 *
 * @return A base decoded file, NULL on failure.
 */
caf_io_file_t *caf_base_decode_file(caf_io_file_t *inf, caf_io_file_t *outf,
									const char *alpha, int bits);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_BASE64_H */
/* caf_data_base64.h ends here */

