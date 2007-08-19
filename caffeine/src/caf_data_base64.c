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
#ifndef lint
static char Id[] = "$Id$";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caf/caf.h"
#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_io_file.h"

#define CAF_B64_INPUT_STREAM_SZ			768
#define CAF_B64_OUTPUT_STREAM_SZ		512
#define CAF_B64_NOUSE_QN				0

#ifndef octet_d
#define octet_d					unsigned char
#endif /* !octet_d */


static const char s_base16_alphabet[] = "0123456789ABCDEF";
static const char s_base32_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
static const char s_base64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const octet_d B64_PAD_CHAR = (octet_d)'=';


static cbuffer_t *s_base_encode (cbuffer_t *buf, const char *codes,
								 const int bits, size_t qn);

static cbuffer_t *s_base_decode (cbuffer_t *buf, const char *codes,
								 const int bits);

static cbuffer_t *s_base_encode_stream (cbuffer_t *buf, const char *codes,
										const int bits, size_t qn,
										cbuffer_t *cache);

static cbuffer_t *s_base_decode_stream (cbuffer_t *buf, const char *codes,
										const int bits, cbuffer_t *cache);

/* === normal encoding/decoding === */
/* --- base 16 --- */
cbuffer_t *
caf_base16_encode (cbuffer_t *in) {
	const int bits = 4;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode (in, s_base16_alphabet, bits,
							 CAF_B64_NOUSE_QN);
	}
	return out;
}


cbuffer_t *
caf_base16_decode (cbuffer_t *in) {
	const int bits = 4;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_decode (in, s_base16_alphabet, bits);
	}
	return out;
}


/* --- base 32 --- */
cbuffer_t *
caf_base32_encode (cbuffer_t *in) {
	const int bits = 5;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode (in, s_base32_alphabet, bits,
							 CAF_B64_NOUSE_QN);
	}
	return out;
}


cbuffer_t *
caf_base32_decode (cbuffer_t *in) {
	const int bits = 5;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_decode (in, s_base32_alphabet, bits);
	}
	return out;
}


cbuffer_t *
caf_base32_encode_complete (cbuffer_t *in) {
	const int bits = 5;
	size_t qn = 40 / 5;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode (in, s_base32_alphabet, bits, qn);
	}
	return out;
}


/* --- base 64 --- */
cbuffer_t *
caf_base64_encode (cbuffer_t *in) {
	const int bits = 6;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode (in, s_base64_alphabet, bits,
							 CAF_B64_NOUSE_QN);
	}
	return out;
}


cbuffer_t *
caf_base64_decode (cbuffer_t *in) {
	const int bits = 6;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_decode (in, s_base64_alphabet, bits);
	}
	return out;
}


cbuffer_t *
caf_base64_encode_complete (cbuffer_t *in) {
	const int bits = 6;
	size_t qn = 24 / 6;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode (in, s_base64_alphabet, bits, qn);
	}
	return out;
}


cbuffer_t *
caf_base64_encode_url (cbuffer_t *in) {
	const int bits = 6;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode (in, s_base64_alphabet, bits,
							 CAF_B64_NOUSE_QN);
	}
	return out;
}


cbuffer_t *
caf_base64_decode_url (cbuffer_t *in) {
	const int bits = 6;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_decode (in, s_base64_alphabet, bits);
	}
	return out;
}


/* === streaming encoding/decoding === */
/* --- base 16 --- */
cbuffer_t *
caf_base16_encode_stream (cbuffer_t *in, cbuffer_t *cache) {
	const int bits = 4;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode_stream (in, s_base16_alphabet, bits,
									CAF_B64_NOUSE_QN, cache);
	}
	return out;
}


cbuffer_t *
caf_base16_decode_stream (cbuffer_t *in, cbuffer_t *cache) {
	const int bits = 4;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_decode_stream (in, s_base16_alphabet, bits, cache);
	}
	return out;
}


/* --- base 32 --- */
cbuffer_t *
caf_base32_encode_stream (cbuffer_t *in, cbuffer_t *cache) {
	const int bits = 5;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode_stream (in, s_base32_alphabet, bits,
									CAF_B64_NOUSE_QN, cache);
	}
	return out;
}


cbuffer_t *
caf_base32_decode_cache (cbuffer_t *in, cbuffer_t *cache) {
	const int bits = 5;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_decode_stream (in, s_base32_alphabet, bits, cache);
	}
	return out;
}


cbuffer_t *
caf_base32_encode_complete_stream (cbuffer_t *in, cbuffer_t *cache) {
	const int bits = 5;
	size_t qn = 40 / 5;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode_stream (in, s_base32_alphabet, bits, qn, cache);
	}
	return out;
}


/* --- base 64 --- */
cbuffer_t *
caf_base64_encode_stream (cbuffer_t *in, cbuffer_t *cache) {
	const int bits = 6;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode_stream (in, s_base64_alphabet, bits,
									CAF_B64_NOUSE_QN, cache);
	}
	return out;
}


cbuffer_t *
caf_base64_decode_stream (cbuffer_t *in, cbuffer_t *cache) {
	const int bits = 6;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_decode_stream (in, s_base64_alphabet, bits, cache);
	}
	return out;
}


cbuffer_t *
caf_base64_encode_complete_stream (cbuffer_t *in, cbuffer_t *cache) {
	const int bits = 6;
	size_t qn = 24 / 6;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = s_base_encode_stream (in, s_base64_alphabet, bits, qn, cache);
	}
	return out;
}


/* === encoding/decoding functions === */
static cbuffer_t *
s_base_encode (cbuffer_t *buf, const char *codes, const int bits, size_t qn) {
	int b8 = 0, i8 = 0;
	octet_d *p_in = 0, *p_out = 0;
	size_t t_in = 0, t_out = 0;
	size_t ind = 0;
	size_t left = 0;
	size_t c = 0;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (buf != (cbuffer_t *)NULL && codes != (const char *)NULL
		&& bits > 0) {
		t_in = (size_t)buf->iosz > 0 ? (size_t)buf->iosz : buf->sz;
		t_out = t_in << 3;
		if ((t_out % (size_t)bits) > 0) {
			t_out = (t_out / bits) + 1;
		} else {
			t_out = (t_out / bits);
		}
		if (qn > 0) {
			ind = t_out % (size_t)qn;
			if (ind > 0) {
				t_out = (t_out + qn) - ind;
			}
		} else {
			ind = 0;
		}
		out = cbuf_create (t_out);
		if (out != (cbuffer_t *)NULL) {
			cbuf_clean (out);
			out->iosz = out->sz;
			p_in = (octet_d *)buf->data;
			p_out = (octet_d *)out->data;
			for (c = 0; c < (size_t)t_out; c++) {
				b8 <<= 8;
				b8 |= *p_in;
				i8 += 8;
				p_in++;
				while (i8 > bits) {
					i8 -= bits;
					*p_out = codes[(b8 >> i8)];
					b8 = b8 - ((b8 >> i8) << i8);
					p_out++;
				}
			}
			if (i8 > 0) {
				*p_out = codes[(b8 << (bits - b8))];
				p_out++;
			}
			if (ind > 0) {
				while (ind > 0) {
					left = (((size_t)p_out) - ind);
					p_out = (octet_d *)left;
					*p_out = B64_PAD_CHAR;
					ind--;
				}
			}
		}
	}
	return out;
}


static cbuffer_t *
s_base_decode (cbuffer_t *buf, const char *codes, const int bits) {
	size_t cnt = 0;
	int b8 = 0, i8 = 0;
	octet_d *p_in = (octet_d *)NULL, *p_out = (octet_d *)NULL;
	size_t t_in = 0, t_out = 0, i = 0;
	int e_codes[255];
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (buf != (cbuffer_t *)NULL && codes != (const char *)NULL
		&& bits > 0) {
		t_in = (size_t)buf->iosz > 0 ? (size_t)buf->iosz : buf->sz;
		t_out = (t_in * bits) >> 3;
		out = cbuf_create (t_out);
		out->iosz = out->sz;
		p_in = (octet_d *)buf->data;
		p_out = (octet_d *)out->data;
		b8 = 0;
		i8 = 0;
		memset (e_codes, 0, 255);
		for (cnt = 0; cnt < strlen(codes); cnt++) {
			e_codes[((int)((octet_d)(codes[cnt])))] = cnt;
		}
		cnt = 0;
		for (i = 1; i <= t_in; i++) {
			if (e_codes[((int)(*p_in))] > 0) {
				b8 <<= bits;
				b8 |= e_codes[((int)(*p_in))];
				i8 += bits;
				while (i8 >= 8) {
					i8 -= 8;
					*p_out = b8 >> i8;
					p_out++;
					cnt++;
				}
				p_in++;
			}
		}
	}

	return out;
}


static cbuffer_t *
s_base_encode_stream (cbuffer_t *buf, const char *codes,
					  const int bits, size_t qn,
					  cbuffer_t *cache) {
	cbuffer_t *nb = (cbuffer_t *)NULL;
	cbuffer_t *out = (cbuffer_t *)NULL;
	size_t cpos_cache = 0, cpos_buf = 0, tsz = 0, ncsz = 0;
	if (buf != (cbuffer_t *)NULL && codes != (const char *)NULL
		&& bits > 0 && cache != (cbuffer_t *)NULL && tsz > 0) {
		cpos_cache = (size_t)cache->iosz > 0 ? (size_t)cache->iosz : cache->sz;
		cpos_buf = (size_t)buf->iosz > 0 ? (size_t)buf->iosz : buf->sz;
		tsz = cpos_cache + cpos_buf;
		ncsz = tsz % 3;
		if (ncsz > 0) {
			nb = cbuf_create (tsz - ncsz);
			cbuf_clean (nb);
			if (cpos_cache > 0) {
				cbuf_paste (nb, cache, 0, 0, cpos_cache);
				cbuf_paste (nb, buf, 0, cpos_cache, ((tsz - ncsz)
													 - cpos_cache));
			} else {
				cbuf_paste (nb, buf, 0, 0, tsz - ncsz);
			}
			cbuf_clean (cache);
			cbuf_paste (cache, buf, 0, ((tsz - ncsz) - cpos_cache), ncsz);
			out = s_base_encode (nb, codes, bits, qn);
			cbuf_delete (nb);
		} else {
			nb = cbuf_create (tsz);
			cbuf_clean (nb);
			if (cpos_cache > 0) {
				cbuf_paste (nb, cache, 0, 0, cpos_cache);
				cbuf_paste (nb, buf, 0, cpos_cache, (tsz - cpos_cache));
			} else {
				cbuf_paste (nb, buf, 0, 0, tsz);
			}
			out = s_base_encode (nb, codes, bits, qn);
			cbuf_delete (nb);
		}
	}
	return out;
}


static cbuffer_t *
s_base_decode_stream (cbuffer_t *buf, const char *codes,
					  const int bits, cbuffer_t *cache) {
	cbuffer_t *nb = (cbuffer_t *)NULL;
	cbuffer_t *out = (cbuffer_t *)NULL;
	size_t cpos_cache = 0, cpos_buf = 0, tsz = 0, ncsz = 0;
	if (buf != (cbuffer_t *)NULL && codes != (const char *)NULL
		&& bits > 0 && cache != (cbuffer_t *)NULL && tsz > 0) {
		cpos_cache = (size_t)cache->iosz > 0 ? (size_t)cache->iosz : cache->sz;
		cpos_buf = (size_t)buf->iosz > 0 ? (size_t)buf->iosz : buf->sz;
		tsz = cpos_cache + cpos_buf;
		ncsz = tsz % 4;
		if (ncsz > 0) {
			nb = cbuf_create (tsz - ncsz);
			cbuf_clean (nb);
			if (cpos_cache > 0) {
				cbuf_paste (nb, cache, 0, 0, cpos_cache);
				cbuf_paste (nb, buf, 0, cpos_cache, ((tsz - ncsz)
													 - cpos_cache));
			} else {
				cbuf_paste (nb, buf, 0, 0, tsz - ncsz);
			}
			cbuf_clean (cache);
			cbuf_paste (cache, buf, 0, ((tsz - ncsz) - cpos_cache), ncsz);
			out = s_base_decode (nb, codes, bits);
			cbuf_delete (nb);
		} else {
			nb = cbuf_create (tsz);
			cbuf_clean (nb);
			if (cpos_cache > 0) {
				cbuf_paste (nb, cache, 0, 0, cpos_cache);
				cbuf_paste (nb, buf, 0, cpos_cache, (tsz - cpos_cache));
			} else {
				cbuf_paste (nb, buf, 0, 0, tsz);
			}
			out = s_base_decode (nb, codes, bits);
			cbuf_delete (nb);
		}
	}
	return out;
}


/* caf_data_base64.c ends here */
