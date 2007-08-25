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

	const char input_string[] = "Hello Encoding/Decoding World!!!";
	char *cache = (char *)NULL;

	cbuffer_t *in_single = (cbuffer_t *)NULL;
	cbuffer_t *in_encode = (cbuffer_t *)NULL;
	cbuffer_t *out_decode = (cbuffer_t *)NULL;

	in_single = cbuf_create ((size_t)(strlen (input_string) + 1));

	if (in_single != (cbuffer_t *)NULL) {

		printf ("in_single: %p\n", (void *)in_single);

		cbuf_clean (in_single);
		cbuf_import (in_single, input_string, strlen (input_string));

		// base16 encoding/decoding
		in_encode = caf_base16_encode (in_single);
		if (in_encode != (cbuffer_t *)NULL) {
			// display encoding
			cache = (char *)xmalloc (in_encode->sz + 1);
			memset (cache, 0, in_encode->sz + 1);
			memcpy (cache, in_encode->data, in_encode->sz);
			printf ("caf_base16_encode: %s\n", cache);
			xfree (cache);
			// decoding and display decoding
			out_decode = caf_base16_decode (in_encode);
			if (out_decode != (cbuffer_t *)NULL) {
				cache = (char *)xmalloc (out_decode->sz + 1);
				memset (cache, 0, out_decode->sz + 1);
				memcpy (cache, out_decode->data, out_decode->sz);
				printf ("caf_base16_decode: %s\n", cache);
				cbuf_delete (out_decode);
				xfree (cache);
			}
			cbuf_delete (in_encode);
		}

		// base32 encoding/decoding
		in_encode = caf_base32_encode (in_single);
		if (in_encode != (cbuffer_t *)NULL) {
			// display encoding
			cache = (char *)xmalloc (in_encode->sz + 1);
			memset (cache, 0, in_encode->sz + 1);
			memcpy (cache, in_encode->data, in_encode->sz);
			printf ("caf_base32_encode: %s\n", cache);
			xfree (cache);
			// decoding and display decoding
			out_decode = caf_base32_decode (in_encode);
			if (out_decode != (cbuffer_t *)NULL) {
				cache = (char *)xmalloc (out_decode->sz + 1);
				memset (cache, 0, out_decode->sz + 1);
				memcpy (cache, out_decode->data, out_decode->sz);
				printf ("caf_base32_decode: %s\n", cache);
				cbuf_delete (out_decode);
				xfree (cache);
			}
			cbuf_delete (in_encode);
		}

		// base64 encoding/decoding
		in_encode = caf_base64_encode (in_single);
		if (in_encode != (cbuffer_t *)NULL) {
			// display encoding
			cache = (char *)xmalloc (in_encode->sz + 1);
			memset (cache, 0, in_encode->sz + 1);
			memcpy (cache, in_encode->data, in_encode->sz);
			printf ("caf_base64_encode: %s\n", cache);
			xfree (cache);
			// decoding and display decoding
			out_decode = caf_base64_decode (in_encode);
			if (out_decode != (cbuffer_t *)NULL) {
				cache = (char *)xmalloc (out_decode->sz + 1);
				memset (cache, 0, out_decode->sz + 1);
				memcpy (cache, out_decode->data, out_decode->sz);
				printf ("caf_base64_decode: %s\n", cache);
				cbuf_delete (out_decode);
				xfree (cache);
			}
			cbuf_delete (in_encode);
		}

		cbuf_delete (in_single);

	}

	exit (EXIT_SUCCESS);
}


/* caf_base64.c ends here */
