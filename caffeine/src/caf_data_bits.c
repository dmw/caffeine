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

#include <sys/types.h>

#include "caf/caf.h"
#include "caf/caf_data_bits.h"

static const u_int16_t u16b1 = 0x00ff;
static const u_int16_t u16b2 = 0xff00;

static const u_int32_t u32b1 = 0x000000ff;
static const u_int32_t u32b2 = 0x0000ff00;
static const u_int32_t u32b3 = 0x00ff0000;
static const u_int32_t u32b4 = 0xff000000;

static const u_int64_t u64b1 = 0x00000000000000ff;
static const u_int64_t u64b2 = 0x000000000000ff00;
static const u_int64_t u64b3 = 0x0000000000ff0000;
static const u_int64_t u64b4 = 0x00000000ff000000;
static const u_int64_t u64b5 = 0x000000ff00000000;
static const u_int64_t u64b6 = 0x0000ff0000000000;
static const u_int64_t u64b7 = 0x00ff000000000000;
static const u_int64_t u64b8 = 0xff00000000000000;


inline u_int16_t
caf_word_endian_swap (u_int16_t w) {
	u_int16_t c1, c2;
	c1 = (w & u16b1) << 8;
	c2 = (w & u16b2) >> 8;
	return (c1 + c2);
}


inline u_int32_t
caf_dword_endian_swap (u_int32_t w) {
	u_int32_t c1, c2, c3, c4;
	c1 = (w & u32b1) << 24;
	c2 = (w & u32b2) << 16;
	c3 = (w & u32b1) >> 16;
	c4 = (w & u32b2) >> 24;
	return (c1 + c2 + c3 + c4);
}


inline u_int64_t
caf_qword_endian_swap (u_int64_t w) {
	u_int32_t c1, c2, c3, c4, c5, c6, c7, c8;
	c1 = (w & u32b1) << 56;
	c2 = (w & u32b2) << 48;
	c3 = (w & u32b1) << 40;
	c4 = (w & u32b2) << 32;
	c5 = (w & u32b1) >> 32;
	c6 = (w & u32b2) >> 40;
	c7 = (w & u32b1) >> 48;
	c8 = (w & u32b2) >> 56;
	return (c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8);
}

/* caf_data_bits.c ends here */
