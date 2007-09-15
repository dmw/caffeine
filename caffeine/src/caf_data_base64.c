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

#include "config.h"
#include "caf/caf.h"
#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_io_file.h"
#include "caf/caf_data_base64.h"

#define CAF_B64_NOUSE_QN				0

#define CAF_B16_BITS					4
#define CAF_B32_BITS					5
#define CAF_B64_BITS					6

#define CAF_BASE_CACHE_SZ				16

#ifndef octet_d
#define octet_d					unsigned char
#endif /* !octet_d */

#ifndef B64_PAD_CHAR
#define B64_PAD_CHAR			(octet_d)'='
#endif /* !B64_PAD_CHAR */

const char caf_base16_alphabet[] =
"0123456789ABCDEF";

const char caf_base32_alphabet[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"234567";

const char caf_base64_alphabet[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

const char caf_base64_alphabet_url[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789-_";

/* === common operations === */
size_t
base_encode_chunk_sz (int bits) {
	size_t chunk_sz = 0;
	if (bits > 0) {
		chunk_sz = 1;
		while (((chunk_sz * bits) % 8) != 0) {
			chunk_sz++;
		}
		chunk_sz--;
	}
	return chunk_sz;
}


size_t
base_decode_chunk_sz (int bits) {
	size_t chunk_sz = 0;
	if (bits > 0) {
		chunk_sz = 1;
		while (((chunk_sz * 8) % bits) != 0) {
			chunk_sz++;
		}
	}
	return ((chunk_sz * 8) / bits);
}


size_t
base_encode_buffer_sz (int bits) {
	size_t tsz = 1;
	size_t csz = base_encode_chunk_sz (bits);
	size_t c = 1;
	while (tsz < 768) {
		tsz = c * csz;
		c++;
	}
	return tsz;
}


size_t
base_decode_buffer_sz (int bits) {
	size_t tsz = 1;
	size_t csz = base_decode_chunk_sz (bits);
	size_t c = 1;
	while (tsz < 768) {
		tsz = c * csz;
		c++;
	}
	return tsz;
}


/* === normal encoding/decoding === */
/* --- base 16 --- */
cbuffer_t *
caf_base16_encode (cbuffer_t *in) {
	const int bits = CAF_B16_BITS;
	size_t qn = 0;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = caf_base_encode (in, caf_base16_alphabet, bits, qn);
	}
	return out;
}


cbuffer_t *
caf_base16_decode (cbuffer_t *in) {
	const int bits = CAF_B16_BITS;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = caf_base_decode (in, caf_base16_alphabet, bits);
	}
	return out;
}


/* --- base 32 --- */
cbuffer_t *
caf_base32_encode (cbuffer_t *in) {
	const int bits = CAF_B32_BITS;
	size_t qn = 40 / CAF_B32_BITS;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = caf_base_encode (in, caf_base32_alphabet, bits, qn);
	}
	return out;
}


cbuffer_t *
caf_base32_decode (cbuffer_t *in) {
	const int bits = CAF_B32_BITS;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = caf_base_decode (in, caf_base32_alphabet, bits);
	}
	return out;
}


/* --- base 64 --- */
cbuffer_t *
caf_base64_encode (cbuffer_t *in) {
	const int bits = CAF_B64_BITS;
	size_t qn = 24 / CAF_B64_BITS;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = caf_base_encode (in, caf_base64_alphabet, bits, qn);
	}
	return out;
}


cbuffer_t *
caf_base64_decode (cbuffer_t *in) {
	const int bits = CAF_B64_BITS;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = caf_base_decode (in, caf_base64_alphabet, bits);
	}
	return out;
}


cbuffer_t *
caf_base64_encode_url (cbuffer_t *in) {
	const int bits = 6;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = caf_base_encode (in, caf_base64_alphabet_url, bits,
							   CAF_B64_NOUSE_QN);
	}
	return out;
}


cbuffer_t *
caf_base64_decode_url (cbuffer_t *in) {
	const int bits = CAF_B64_BITS;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (in != (cbuffer_t *)NULL) {
		out = caf_base_decode (in, caf_base64_alphabet_url, bits);
	}
	return out;
}


/* === encoding/decoding files === */
/* --- base 16 --- */
caf_io_file_t *
caf_base16_encode_file (caf_io_file_t *inf, caf_io_file_t *outf) {
	caf_io_file_t *r = (caf_io_file_t *)NULL;
	size_t qn = 0;
	if (inf != (caf_io_file_t *)NULL && outf != (caf_io_file_t *)NULL) {
		r = caf_base_encode_file (inf, outf, caf_base16_alphabet, CAF_B16_BITS, qn);
	}
	return r;
}


caf_io_file_t *
caf_base16_decode_file (caf_io_file_t *inf, caf_io_file_t *outf) {
	caf_io_file_t *r = (caf_io_file_t *)NULL;
	if (inf != (caf_io_file_t *)NULL && outf != (caf_io_file_t *)NULL) {
		r = caf_base_decode_file (inf, outf, caf_base16_alphabet, CAF_B16_BITS);
	}
	return r;
}


/* --- base 32 --- */
caf_io_file_t *
caf_base32_encode_file (caf_io_file_t *inf, caf_io_file_t *outf) {
	caf_io_file_t *r = (caf_io_file_t *)NULL;
	size_t qn = 40 / CAF_B32_BITS;
	if (inf != (caf_io_file_t *)NULL && outf != (caf_io_file_t *)NULL) {
		r = caf_base_encode_file (inf, outf, caf_base32_alphabet, CAF_B32_BITS, qn);
	}
	return r;
}


caf_io_file_t *
caf_base32_decode_file (caf_io_file_t *inf, caf_io_file_t *outf) {
	caf_io_file_t *r = (caf_io_file_t *)NULL;
	if (inf != (caf_io_file_t *)NULL && outf != (caf_io_file_t *)NULL) {
		r = caf_base_decode_file (inf, outf, caf_base32_alphabet, CAF_B32_BITS);
	}
	return r;
}


/* --- base 64 --- */
caf_io_file_t *
caf_base64_encode_file (caf_io_file_t *inf, caf_io_file_t *outf) {
	caf_io_file_t *r = (caf_io_file_t *)NULL;
	size_t qn = 24 / CAF_B64_BITS;
	if (inf != (caf_io_file_t *)NULL && outf != (caf_io_file_t *)NULL) {
		r = caf_base_encode_file (inf, outf, caf_base64_alphabet, CAF_B64_BITS, qn);
	}
	return r;
}


caf_io_file_t *
caf_base64_decode_file (caf_io_file_t *inf, caf_io_file_t *outf) {
	caf_io_file_t *r = (caf_io_file_t *)NULL;
	if (inf != (caf_io_file_t *)NULL && outf != (caf_io_file_t *)NULL) {
		r = caf_base_decode_file (inf, outf, caf_base64_alphabet, CAF_B64_BITS);
	}
	return r;
}


/* === encoding/decoding functions === */
cbuffer_t *
caf_base_encode (cbuffer_t *buf, const char *codes, const int bits, size_t qn) {
	int b8 = 0, i8 = 0;
	octet_d *p_in = 0, *p_out = 0;
	size_t t_in = 0, t_out = 0;
	size_t ind = 0;
	size_t left = 0;
	size_t c = 0;
	size_t encode_chunk_sz = 0;
	size_t decode_chunk_sz = 0;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (buf != (cbuffer_t *)NULL && codes != (const char *)NULL
		&& bits > 0) {
		encode_chunk_sz = base_encode_chunk_sz (bits);
		decode_chunk_sz = base_decode_chunk_sz (bits);
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
			p_in = (octet_d *)buf->data;
			p_out = (octet_d *)out->data;
			for (c = 1; c <= t_in; c++) {
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
				*p_out = codes[(b8 << (bits - i8))];
				p_out++;
			}
			if (ind > 0) {
				left = (((size_t)t_out) - (qn - ind));
				while (left < t_out) {
					*p_out = B64_PAD_CHAR;
					p_out++;
					left++;
					out->iosz += 1;
				}
			}
			out->iosz = out->sz;
		}
	}
	return out;
}


cbuffer_t *
caf_base_decode (cbuffer_t *buf, const char *codes, const int bits) {
	size_t cnt = 0;
	int b8 = 0, i8 = 0;
	octet_d *p_in = (octet_d *)NULL, *p_out = (octet_d *)NULL;
	size_t t_in = 0, t_out = 0, i = 0;
	size_t en_sz = 0, de_sz = 0;
	int e_codes[255];
	int value = 0;
	cbuffer_t *out = (cbuffer_t *)NULL;
	if (buf != (cbuffer_t *)NULL && codes != (const char *)NULL
		&& bits > 0) {
		en_sz = base_encode_chunk_sz (bits);
		de_sz = base_decode_chunk_sz (bits);
		t_in = (size_t)buf->iosz > 0 ? (size_t)buf->iosz : buf->sz;
		t_out = (t_in * bits) >> 3;
		out = cbuf_create (t_out);
		if (out != (cbuffer_t *)NULL) {
			cbuf_clean (out);
			p_in = (octet_d *)buf->data;
			p_out = (octet_d *)out->data;
			b8 = 0;
			i8 = 0;
			memset (e_codes, -1, 255);
			for (value = 0; value < (int)strlen (codes); value++) {
				e_codes[((int)(codes[value]))] = value;
			}
			cnt = 0;
			for (i = 0; i < t_in; i++) {
				if (e_codes[((int)(*p_in))] >= 0) {
					b8 <<= bits;
					b8 |= e_codes[((int)(*p_in))];
					i8 += bits;
					while (i8 >= 8) {
						i8 -= 8;
						*p_out = (octet_d)(b8 >> i8);
						p_out++;
						cnt++;
					}
					p_in++;
				}
			}
			out->iosz = t_out;
		}
	}
	return out;
}


cbuffer_t *
caf_base_encode_stream (cbuffer_t *buf, const char *codes, const int bits,
						size_t qn, cbuffer_t *cache) {
	cbuffer_t *out = (cbuffer_t *)NULL;
	cbuffer_t *cb = (cbuffer_t *)NULL;
	size_t tsz = 0, nsz = 0;
	size_t ce_sz = 0;
	size_t c_sz = 0;
	void *cptr = (void *)NULL;
	if (buf != (cbuffer_t *)NULL && codes != (const char *)NULL
		&& bits > 0 && cache != (cbuffer_t *)NULL) {
		ce_sz = base_encode_chunk_sz (bits);
		tsz = (buf->iosz > 0 ? (size_t)buf->iosz : buf->sz) + cache->iosz;
		cb = cbuf_create (tsz);
		cbuf_clean (cb);
		if (cb != (cbuffer_t *)NULL) {
			if ((tsz % ce_sz) != 0) {
				c_sz = (ssize_t)(tsz % ce_sz);
			}
			nsz = tsz - c_sz;
			if (cache->iosz > 0) {
				cbuf_paste (cb, cache, 0, 0, (size_t)cache->iosz);
				cbuf_paste (cb, buf, (size_t)cache->iosz + 1, 0, nsz);
				cb->iosz = ((size_t)cache->iosz + nsz);
			} else {
				cbuf_paste (cb, buf, 0, 0, nsz);
				cb->iosz = nsz;
			}
			if (c_sz > 0) {
				cbuf_clean (cache);
				cptr = (void *)((((size_t)buf->data) + (size_t)buf->iosz)
								- c_sz);
				cbuf_import (cache, cptr, c_sz);
				cache->iosz = c_sz;
			}
			out = caf_base_encode (cb, codes, bits, qn);
			cbuf_delete (cb);
		}
	}
	return out;
}


cbuffer_t *
caf_base_decode_stream (cbuffer_t *buf, const char *codes, const int bits,
						cbuffer_t *cache) {
	cbuffer_t *out = (cbuffer_t *)NULL;
	cbuffer_t *cb = (cbuffer_t *)NULL;
	size_t tsz = 0, nsz = 0;
	size_t ce_sz = 0;
	size_t cd_sz = 0;
	size_t c_sz = 0;
	void *cptr = (void *)NULL;
	if (buf != (cbuffer_t *)NULL && codes != (const char *)NULL
		&& bits > 0 && cache != (cbuffer_t *)NULL) {
		ce_sz = base_encode_chunk_sz (bits);
		cd_sz = base_decode_chunk_sz (bits);
		tsz = (buf->iosz > 0 ? (size_t)buf->iosz : buf->sz) + cache->iosz;
		cb = cbuf_create (tsz);
		cbuf_clean (cb);
		if (cb != (cbuffer_t *)NULL) {
			if ((tsz % cd_sz) != 0) {
				c_sz = (size_t)(tsz % cd_sz);
			}
			nsz = tsz - c_sz;
			if (cache->iosz > 0) {
				cbuf_paste (cb, cache, 0, 0, (size_t)cache->iosz);
				cbuf_paste (cb, buf, (size_t)cache->iosz, 0, nsz);
				cb->iosz = ((size_t)cache->iosz + nsz);				
			} else {
				cbuf_paste (cb, buf, 0, 0, nsz);
				cb->iosz = nsz;
			}
			if (c_sz > 0) {
				cbuf_clean (cache);
				cptr = (void *)((((size_t)buf->data) + (size_t)buf->iosz)
								- c_sz);
				cbuf_import (cache, cptr, c_sz);
				cache->iosz = c_sz;
			}
			out = caf_base_decode (cb, codes, bits);
			cbuf_delete (cb);
		}
	}
	return out;
}


caf_io_file_t *
caf_base_encode_file (caf_io_file_t *inf, caf_io_file_t *outf,
					  const char *alpha, int bits, size_t qn) {
	caf_io_file_t *r = (caf_io_file_t *)NULL;
	cbuffer_t *cache = (cbuffer_t *)NULL;
	cbuffer_t *inb = (cbuffer_t *)NULL;
	cbuffer_t *outb = (cbuffer_t *)NULL;
	int encoded = 0;
	if (inf != (caf_io_file_t *)NULL) {
		if ((io_restat (inf)) == CAF_OK && (io_restat (outf)) == CAF_OK) {
			cache = cbuf_create (CAF_BASE_CACHE_SZ);
			cbuf_clean (cache);
			inb = cbuf_create (base_encode_buffer_sz (bits));
			if (inb == (cbuffer_t *)NULL || cache == (cbuffer_t *)NULL) {
				cbuf_delete (cache);
				return r;
			}
			cbuf_clean (inb);
			io_flseek (inf, 0, SEEK_SET);
			while ((io_read (inf, inb)) > 0) {
				outb = caf_base_encode_stream (inb, alpha, bits, qn, cache);
				encoded = 0;
				if (outb != (cbuffer_t *)NULL) {
					if ((io_write (outf, outb)) > 0) {
						encoded = 1;
					}
					cbuf_delete (outb);
				}
				io_restat (inf);
			}
			cbuf_delete (inb);
			if (encoded > 0) {
				r = outf;
			}
		}
	}
	return r;
}


caf_io_file_t *
caf_base_decode_file (caf_io_file_t *inf, caf_io_file_t *outf,
					  const char *alpha, int bits) {
	caf_io_file_t *r = (caf_io_file_t *)NULL;
	cbuffer_t *cache = (cbuffer_t *)NULL;
	cbuffer_t *inb = (cbuffer_t *)NULL;
	cbuffer_t *outb = (cbuffer_t *)NULL;
	int encoded = 0;
	if (inf != (caf_io_file_t *)NULL) {
		if ((io_restat (inf)) == CAF_OK && (io_restat (outf)) == CAF_OK) {
			cache = cbuf_create (CAF_BASE_CACHE_SZ);
			cbuf_clean (cache);
			inb = cbuf_create (base_decode_buffer_sz (bits));
			if (inb == (cbuffer_t *)NULL || cache == (cbuffer_t *)NULL) {
				cbuf_delete (cache);
				return r;
			}
			cbuf_clean (inb);
			io_flseek (inf, 0, SEEK_SET);
			while ((io_read (inf, inb)) > 0) {
				outb = caf_base_decode_stream (inb, alpha, bits, cache);
				if (outb != (cbuffer_t *)NULL) {
					if ((io_write (outf, outb)) > 0) {
						encoded = 1;
					}
					cbuf_delete (outb);
				}
			}
			cbuf_delete (inb);
			if (encoded > 0) {
				r = outf;
			}
		}
	}
	return r;
}


/* caf_data_base64.c ends here */
