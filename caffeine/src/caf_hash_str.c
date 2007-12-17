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
#include "caf/config.h"
#endif /* !HAVE_CONFIG_H */

#include <stdlib.h>
#include <sys/types.h>

#include "caf/caf_hash_str.h"

u_int32_t
caf_shash_rs (const char *str, const u_int len) {
	/* Modified Robert Sedgwicks String Hash Algorithm */
	u_int32_t b = 378551;
	u_int32_t a = 63689;
	u_int32_t hash = 0;
	u_int32_t i = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash = hash * a + (*i32p);
				a    = a * b;
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash = hash * a + (*ptr);
					a    = a * b;
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash = hash * a + (*ptr);
				a    = a * b;
			}
		}
		return hash;
	}
	return 0;
}


u_int
caf_shash_js (const char* str, const u_int len) {
	/* Modified Justin Sobel String Hash Algorithm */
	u_int32_t hash = 1315423911;
	u_int32_t i = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash ^= ((hash << 5) + (*i32p) + (hash >> 2));
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash ^= ((hash << 5) + (u_int32_t)(*ptr) + (hash >> 2));
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash ^= ((hash << 5) + (u_int32_t)(*ptr) + (hash >> 2));
			}
		}
		return hash;
	}
	return 0;
}


u_int
caf_shash_pjw (const char* str, const u_int len) {
	/* Modified Peter J. Weinberger String Hash Algorithm */
	const u_int32_t i32_bits = (u_int32_t)(sizeof(u_int32_t) * 8);
	const u_int32_t i32_34 = (u_int32_t)((i32_bits  * 3) / 4);
	const u_int32_t i32_8 = (u_int32_t)(i32_bits / 8);
	const u_int32_t i32_hb = (u_int32_t)(0xFFFFFFFF) << (i32_bits - i32_8);
	u_int32_t hash = 0;
	u_int32_t test = 0;
	u_int32_t i = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash = (hash << i32_8) + (*i32p);
				if ((test = hash & i32_hb)  != 0) {
					hash = (( hash ^ (test >> i32_34)) & (~i32_hb));
				}
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash = (hash << i32_8) + (u_int32_t)(*ptr);
					if ((test = hash & i32_hb)  != 0) {
						hash = (( hash ^ (test >> i32_34)) & (~i32_hb));
					}
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash = (hash << i32_8) + (u_int32_t)(*ptr);
				if ((test = hash & i32_hb)  != 0) {
					hash = (( hash ^ (test >> i32_34)) & (~i32_hb));
				}
			}
		}
		return hash;
	}
	return 0;
}


u_int
caf_shash_elf (const char* str, const u_int len) {
	/* Modified ELF String Hash Algorithm */
	u_int32_t hash = 0;
	u_int32_t x    = 0;
	u_int32_t i    = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash = (hash << 4) + (u_int32_t)(*i32p);
				if ((x = hash & 0xF0000000L) != 0) {
					hash ^= (x >> 24);
				}
				hash &= ~x;
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash = (hash << 4) + (u_int32_t)(*ptr);
					if ((x = hash & 0xF0000000L) != 0) {
						hash ^= (x >> 24);
					}
					hash &= ~x;
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash = (hash << 4) + (u_int32_t)(*ptr);
				if ((x = hash & 0xF0000000L) != 0) {
					hash ^= (x >> 24);
				}
				hash &= ~x;
			}
		}
		return hash;
	}
	return 0;
}


u_int
caf_shash_bkdr (const char* str, const u_int len) {
	/* Modified Brian Kernighan and Dennis Ritchie String Hash Algorithm */
	u_int32_t seed = 131;
	u_int32_t hash = 0;
	u_int32_t i    = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash = (hash * seed) + (u_int32_t)(*i32p);
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash = (hash * seed) + (u_int32_t)(*ptr);
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash = (hash * seed) + (u_int32_t)(*ptr);
			}
		}
		return hash;
	}
	return 0;
}


u_int
caf_shash_sdbm (const char* str, const u_int len) {
	/* Modified SDBM String Hash Algorithm */
	u_int32_t hash = 0;
	u_int32_t i    = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash = (u_int32_t)(*i32p) + (hash << 6) + (hash << 16) - hash;
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash = (u_int32_t)(*ptr) + (hash << 6) +
						(hash << 16) - hash;
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash = (u_int32_t)(*ptr) + (hash << 6) +
					(hash << 16) - hash;
			}
		}
		return hash;
	}
	return 0;
}


u_int
caf_shash_djb (const char* str, const u_int len) {
	/* Modified Daniel J. Bernstein String Hash Algorithm */
	u_int32_t hash = 5381;
	u_int32_t i    = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash = ((hash << 5) + hash) + (u_int32_t)(*i32p);
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash = ((hash << 5) + hash) + (u_int32_t)(*ptr);
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash = ((hash << 5) + hash) + (u_int32_t)(*ptr);
			}
		}
		return hash;
	}
	return 0;
}


u_int
caf_shash_dek (const char* str, const u_int len) {
	/* Modified Donald E. Knuth String Hash Algorithm */
	u_int32_t hash = len;
	u_int32_t i    = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash = ((hash << 5) ^ (hash >> 27)) ^ (u_int32_t)(*i32p);
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash = ((hash << 5) ^ (hash >> 27)) ^ (u_int32_t)(*ptr);
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash = ((hash << 5) ^ (hash >> 27)) ^ (u_int32_t)(*ptr);
			}
		}
		return hash;
	}
	return 0;
}


u_int
caf_shash_bp (const char* str, const u_int len) {
	u_int32_t hash = 0;
	u_int32_t i    = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash = hash << 7 ^ (u_int32_t)(*i32p);
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash = hash << 7 ^ (u_int32_t)(*ptr);
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash = hash << 7 ^ (u_int32_t)(*ptr);
			}
		}
		return hash;
	}
	return 0;
}


u_int
caf_shash_fnv (const char* str, const u_int len) {
	const u_int32_t prime = 0x811C9DC5;
	u_int32_t hash = 0;
	u_int32_t i    = 0;
	u_int32_t i32b = 0;
	u_int32_t *i32p = (u_int32_t *)NULL;
	char *ptr = (char *)str;
	if (ptr != (const char *)NULL && len > 0) {
		i32b = len / 4;
		if (i32b > 0) {
			i32p = (u_int32_t *)ptr;
			for (i = 0; i < i32b; i32p++, i++) {
				hash *= prime;
				hash ^= (u_int32_t)(*i32p);
			}
			if ((len % 4) > 0) {
				i32b *= 4;
				ptr = (char *)((size_t)i32b + (size_t)ptr);
				for (i = (len - (len % 4)); i < len; ptr++, i++) {
					hash *= prime;
					hash ^= (u_int32_t)(*ptr);
				}
			}
		} else {
			for (i = 0; i < len; ptr++, i++) {
				hash *= prime;
				hash ^= (u_int32_t)(*ptr);
			}
		}
		return hash;
	}
	return 0;
}

/* caf_hash_str.c ends here */
