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
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>

#include "caf/caf.h"
#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_data_base64.h"
#include "caf/caf_io.h"
#include "caf/caf_io_file.h"


int
main (void) {

	const char input_string[] = "Hello Base Encoding/Decoding World!!!";
	const char long_string[] =
		"\t0. This is a very long message\n"
		"\t1. This is a very long message\n"
		"\t2. This is a very long message\n"
		"\t3. This is a very long message\n"
		"\t4. This is a very long message\n"
		"\t5. This is a very long message\n"
		"\t6. This is a very long message\n"
		"\t7. This is a very long message\n"
		"\t8. This is a very long message\n"
		"\t9. This is a very long message\n"
		"\t0. This is a very long message\n"
		"\t1. This is a very long message\n"
		"\t2. This is a very long message\n"
		"\t3. This is a very long message\n"
		"\t4. This is a very long message\n"
		"\t5. This is a very long message\n"
		"\t6. This is a very long message\n"
		"\t7. This is a very long message\n"
		"\t8. This is a very long message\n"
		"\t9. This is a very\nlong message\n";
	char *cache = (char *)NULL;

	cbuffer_t *in_single = (cbuffer_t *)NULL;
	cbuffer_t *in_encode = (cbuffer_t *)NULL;
	cbuffer_t *out_decode = (cbuffer_t *)NULL;

	printf ("base 64 chunk size: in=%d, out=%d\n",
			base_encode_chunk_sz (6),
			base_decode_chunk_sz (6));

	printf ("base 32 chunk size: in=%d, out=%d\n",
			base_encode_chunk_sz (5),
			base_decode_chunk_sz (5));

	printf ("base 16 chunk size: in=%d, out=%d\n",
			base_encode_chunk_sz (4),
			base_decode_chunk_sz (4));

	printf ("base 64 buffer size: in=%d, out=%d\n",
			base_encode_buffer_sz (6),
			base_decode_buffer_sz (6));

	printf ("base 32 buffer size: in=%d, out=%d\n",
			base_encode_buffer_sz (5),
			base_decode_buffer_sz (5));

	printf ("base 16 buffer size: in=%d, out=%d\n",
			base_encode_buffer_sz (4),
			base_decode_buffer_sz (4));

	printf ("==================================================\n");

	in_single = cbuf_create ((size_t)(strlen (input_string) + 1));

	if (in_single != (cbuffer_t *)NULL) {

		printf ("input: %s (len:%d; %p)\n", input_string,
				strlen (input_string), (void *)in_single);

		cbuf_clean (in_single);
		cbuf_import (in_single, input_string, strlen (input_string));

		// base16 encoding/decoding
		in_encode = caf_base16_encode (in_single);
		if (in_encode != (cbuffer_t *)NULL) {
			// display encoding
			cache = (char *)xmalloc (in_encode->sz + 1);
			memset (cache, 0, in_encode->sz + 1);
			memcpy (cache, in_encode->data, in_encode->sz);
			printf ("base16 encode: %s\n", cache);
			xfree (cache);
			// decoding and display decoding
			out_decode = caf_base16_decode (in_encode);
			if (out_decode != (cbuffer_t *)NULL) {
				cache = (char *)xmalloc (out_decode->iosz + 1);
				memset (cache, 0, out_decode->sz + 1);
				memcpy (cache, out_decode->data, out_decode->sz);
				printf ("base16 decode: %s (len:%d; blen:%d)\n", cache,
						strlen (cache), (int)out_decode->iosz);
				cbuf_delete (out_decode);
				xfree (cache);
			}
			cbuf_delete (in_encode);
		}

		printf ("==================================================\n");

		// base32 encoding/decoding
		in_encode = caf_base32_encode (in_single);
		if (in_encode != (cbuffer_t *)NULL) {
			// display encoding
			cache = (char *)xmalloc (in_encode->sz + 1);
			memset (cache, 0, in_encode->sz + 1);
			memcpy (cache, in_encode->data, in_encode->sz);
			printf ("base32 encode: %s\n", cache);
			xfree (cache);
			// decoding and display decoding
			out_decode = caf_base32_decode (in_encode);
			if (out_decode != (cbuffer_t *)NULL) {
				cache = (char *)xmalloc (out_decode->iosz + 1);
				memset (cache, 0, out_decode->sz + 1);
				memcpy (cache, out_decode->data, out_decode->sz);
				printf ("base32 decode: %s (len:%d; blen:%d)\n", cache,
						strlen (cache), (int)out_decode->iosz);
				cbuf_delete (out_decode);
				xfree (cache);
			}
			cbuf_delete (in_encode);
		}

		printf ("==================================================\n");

		// base64 encoding/decoding
		in_encode = caf_base64_encode (in_single);
		if (in_encode != (cbuffer_t *)NULL) {
			// display encoding
			cache = (char *)xmalloc (in_encode->sz + 1);
			memset (cache, 0, in_encode->sz + 1);
			memcpy (cache, in_encode->data, in_encode->sz);
			printf ("base64 encode: %s\n", cache);
			xfree (cache);
			// decoding and display decoding
			out_decode = caf_base64_decode (in_encode);
			if (out_decode != (cbuffer_t *)NULL) {
				cache = (char *)xmalloc (out_decode->iosz + 1);
				memset (cache, 0, out_decode->sz + 1);
				memcpy (cache, out_decode->data, out_decode->sz);
				printf ("base64 decode: %s (len:%d; blen:%d)\n", cache,
						strlen (cache), (int)out_decode->iosz);
				cbuf_delete (out_decode);
				xfree (cache);
			}
			cbuf_delete (in_encode);
		}

		printf ("==================================================\n");

		cbuf_delete (in_single);

	}


	in_single = cbuf_create ((size_t)(strlen (long_string) + 1));

	if (in_single != (cbuffer_t *)NULL) {

		printf ("input:\n%s (len:%d; %p)\n", long_string,
				strlen (long_string), (void *)in_single);

		cbuf_clean (in_single);
		cbuf_import (in_single, long_string, strlen (long_string));

		// base16 encoding/decoding
		in_encode = caf_base16_encode (in_single);
		if (in_encode != (cbuffer_t *)NULL) {
			// display encoding
			cache = (char *)xmalloc (in_encode->sz + 1);
			memset (cache, 0, in_encode->sz + 1);
			memcpy (cache, in_encode->data, in_encode->sz);
			printf ("base16 encode: %s\n", cache);
			xfree (cache);
			// decoding and display decoding
			out_decode = caf_base16_decode (in_encode);
			if (out_decode != (cbuffer_t *)NULL) {
				cache = (char *)xmalloc (out_decode->iosz + 1);
				memset (cache, 0, out_decode->sz + 1);
				memcpy (cache, out_decode->data, out_decode->sz);
				printf ("base16 decode: %s (len:%d; blen:%d)\n", cache,
						strlen (cache), (int)out_decode->iosz);
				cbuf_delete (out_decode);
				xfree (cache);
			}
			cbuf_delete (in_encode);
		}

		printf ("==================================================\n");

		// base32 encoding/decoding
		in_encode = caf_base32_encode (in_single);
		if (in_encode != (cbuffer_t *)NULL) {
			// display encoding
			cache = (char *)xmalloc (in_encode->sz + 1);
			memset (cache, 0, in_encode->sz + 1);
			memcpy (cache, in_encode->data, in_encode->sz);
			printf ("base32 encode: %s\n", cache);
			xfree (cache);
			// decoding and display decoding
			out_decode = caf_base32_decode (in_encode);
			if (out_decode != (cbuffer_t *)NULL) {
				cache = (char *)xmalloc (out_decode->iosz + 1);
				memset (cache, 0, out_decode->sz + 1);
				memcpy (cache, out_decode->data, out_decode->sz);
				printf ("base32 decode: %s (len:%d; blen:%d)\n", cache,
						strlen (cache), (int)out_decode->iosz);
				cbuf_delete (out_decode);
				xfree (cache);
			}
			cbuf_delete (in_encode);
		}

		printf ("==================================================\n");

		// base64 encoding/decoding
		in_encode = caf_base64_encode (in_single);
		if (in_encode != (cbuffer_t *)NULL) {
			// display encoding
			cache = (char *)xmalloc (in_encode->sz + 1);
			memset (cache, 0, in_encode->sz + 1);
			memcpy (cache, in_encode->data, in_encode->sz);
			printf ("base64 encode: %s\n", cache);
			xfree (cache);
			// decoding and display decoding
			out_decode = caf_base64_decode (in_encode);
			if (out_decode != (cbuffer_t *)NULL) {
				cache = (char *)xmalloc (out_decode->iosz + 1);
				memset (cache, 0, out_decode->sz + 1);
				memcpy (cache, out_decode->data, out_decode->sz);
				printf ("base64 decode: %s (len:%d; blen:%d)\n", cache,
						strlen (cache), (int)out_decode->iosz);
				cbuf_delete (out_decode);
				xfree (cache);
			}
			cbuf_delete (in_encode);
		}

		printf ("==================================================\n");

		cbuf_delete (in_single);

	}

	return 0;
}


/* caf_base64.c ends here */
