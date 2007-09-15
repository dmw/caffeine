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

#include <sys/types.h>
#include <sys/stat.h>
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

	caf_io_file_t *inf = (caf_io_file_t *)NULL;
	caf_io_file_t *outf = (caf_io_file_t *)NULL;
	caf_io_file_t *r = (caf_io_file_t *)NULL;
	
	inf = io_fopen ("caf_tail.c", O_RDONLY, 0644, CAF_OK);
	if (inf != (caf_io_file_t *)NULL) {
		io_flseek (inf, 0, SEEK_SET);
		outf = io_fopen ("caf_tail.b64", O_WRONLY | O_CREAT | O_TRUNC, 0644, CAF_OK);
		if (outf != (caf_io_file_t *)NULL) {
			r = caf_base64_encode_file (inf, outf);
			if (r != (caf_io_file_t *)NULL) {
				printf ("success on caf_tail.c to base64 encoding\n");
			}
			io_fclose (outf);
		}
		io_flseek (inf, 0, SEEK_SET);
		outf = io_fopen ("caf_tail.b32", O_WRONLY | O_CREAT | O_TRUNC, 0644, CAF_OK);
		if (outf != (caf_io_file_t *)NULL) {
			r = caf_base32_encode_file (inf, outf);
			if (r != (caf_io_file_t *)NULL) {
				printf ("success on caf_tail.c to base32 encoding\n");
			}
			io_fclose (outf);
		}
		io_flseek (inf, 0, SEEK_SET);
		outf = io_fopen ("caf_tail.b16", O_WRONLY | O_CREAT | O_TRUNC, 0644, CAF_OK);
		if (outf != (caf_io_file_t *)NULL) {
			r = caf_base16_encode_file (inf, outf);
			if (r != (caf_io_file_t *)NULL) {
				printf ("success on caf_tail.c to base16 encoding\n");
			}
			io_fclose (outf);
		}
		io_fclose (inf);
	}

	inf = io_fopen ("caf_tail.b64", O_RDONLY, 0644, CAF_OK);
	if (inf != (caf_io_file_t *)NULL) {
		io_flseek (inf, 0, SEEK_SET);
		outf = io_fopen ("caf_tail.b64.c", O_WRONLY | O_CREAT | O_TRUNC, 0644, CAF_OK);
		if (outf != (caf_io_file_t *)NULL) {
			r = caf_base64_decode_file (inf, outf);
			if (r != (caf_io_file_t *)NULL) {
				printf ("success on caf_tail.c to base64 decoding\n");
			}
			io_fclose (outf);
		}
		io_fclose (inf);
	}

	inf = io_fopen ("caf_tail.b32", O_RDONLY, 0644, CAF_OK);
	if (inf != (caf_io_file_t *)NULL) {
		io_flseek (inf, 0, SEEK_SET);
		outf = io_fopen ("caf_tail.b32.c", O_WRONLY | O_CREAT | O_TRUNC, 0644, CAF_OK);
		if (outf != (caf_io_file_t *)NULL) {
			r = caf_base32_decode_file (inf, outf);
			if (r != (caf_io_file_t *)NULL) {
				printf ("success on caf_tail.c to base32 decoding\n");
			}
			io_fclose (outf);
		}
		io_fclose (inf);
	}

	inf = io_fopen ("caf_tail.b16", O_RDONLY, 0644, CAF_OK);
	if (inf != (caf_io_file_t *)NULL) {
		io_flseek (inf, 0, SEEK_SET);
		outf = io_fopen ("caf_tail.b16.c", O_WRONLY | O_CREAT | O_TRUNC, 0644, CAF_OK);
		if (outf != (caf_io_file_t *)NULL) {
			r = caf_base16_decode_file (inf, outf);
			if (r != (caf_io_file_t *)NULL) {
				printf ("success on caf_tail.c to base16 decoding\n");
			}
			io_fclose (outf);
		}
		io_fclose (inf);
	}

	return 0;
}


/* caf_base64.c ends here */
