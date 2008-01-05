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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caf/caf_data_mem.h"

#ifndef CAFFEINE_DEBUG

void *
xmalloc (size_t mem) {
	void *ptr;
	if ((int)mem) {
		ptr = (void *)malloc (mem);
		return ptr;
	} else {
		return (void *)NULL;
	}
}


void
xempty (void *ptr, size_t sz) {
	if (((void *)ptr != (void *)NULL) && (sz > (size_t)0)) {
		memset (ptr, (int)NULL, sz);
	}
}


void
xfree (void *ptr) {
	if (ptr != (void *)NULL) {
		free (ptr);
		ptr = (void *)NULL;
	}
}


void
xdestroy (void *ptr, size_t sz) {
	xempty(ptr, sz);
	xfree(ptr);
}


void *
xrealloc (void *ptr, size_t sz) {
	void *new;
	new = realloc(ptr, sz);
	return new;
}


void
xstrdestroy (void *ptr) {
	size_t mem;
	if (ptr != (void *)NULL) {
		mem = STRSZ(ptr);
		xdestroy(ptr, mem);
	}
}


void *
xmemcpy (void *dst, void *src, size_t sz) {
	if (dst != (void *)NULL && src != (void *)NULL && sz > (long)NULL) {
		return memcpy(dst, src, sz);
	}
	return (void *)NULL;
}

#endif /* !CAFFEINE_DEBUG */
/* caf_data_mem.c ends here */

