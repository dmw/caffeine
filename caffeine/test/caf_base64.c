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
main (int argc, char **argv) {

	cbuffer_t padding;
	padding.iosz = 0;
	padding.sz = 1;
	padding.data = "\n";

	char msg[] =
		"%s usage:\n"
		"%s <operation> <input-file> <output-file>\n"
		"operations:\n"
		"		-encode\n"
		"		-decode\n";

	char msgdo[] =
		"%s: using operation %s over %s\n";

	char *nout = (char *)NULL;

	caf_io_file_t *inf = (caf_io_file_t *)NULL;
	caf_io_file_t *outf = (caf_io_file_t *)NULL;

	char *op = (char *)NULL;
	char *infn = (char *)NULL;
	char *outfn = (char *)NULL;

	size_t sz = 0;

	if (argc < 4) {
		printf (msg, argv[0], argv[0]);
		exit (EXIT_FAILURE);
	}

	op = argv[1];
	infn = argv[2];
	outfn = argv[3];

	printf (msgdo, argv[0], op, infn);

	inf = io_fopen (infn, O_RDONLY, 0644, CAF_OK);
	if (inf != (caf_io_file_t *)NULL) {
		outf = io_fopen (outfn, O_CREAT | O_RDWR | O_TRUNC, 0644, CAF_OK);
		if (outf != (caf_io_file_t *)NULL) {
			printf ("%s: open %s (%p) and %s (%p)\n", argv[0],
					infn, (void *)inf, outfn, (void *)outf);
			if ((caf_base64_encode_file (outf, inf, &padding, 64))
				!= outf) {
				printf ("%s: error encoding %s\n", argv[0], infn);
			} else {
				printf ("%s: encoding success on %s\n", argv[0], infn);
			}
			io_fclose (outf);
		} else {
			printf ("%s: error opening %s\n", argv[0], outfn);
		}
		io_fclose (inf);
	} else {
		printf ("%s: error opening %s\n", argv[0], infn);
	}

	sz = strlen(outfn) + strlen(".new") + 1;
	nout = (char *)xmalloc (sz);
	if (nout != (char *)NULL) {
		memset (nout, 0, sz);
		memcpy (nout, outfn, strlen(outfn));
		nout = strcat (nout, ".new");
		inf = io_fopen (outfn, O_RDONLY, 0644, CAF_OK);
		if (inf != (caf_io_file_t *)NULL) {
			outf = io_fopen (nout, O_CREAT | O_RDWR | O_TRUNC, 0644, CAF_OK);
			if (outf != (caf_io_file_t *)NULL) {
				if ((caf_base64_decode_file (outf, inf)) != outf) {
					printf ("%s: error decoding %s\n", argv[0], infn);
				} else {
					
					printf ("%s: decoding success on %s\n", argv[0], infn);
				}
				io_fclose (outf);
			} else {
				printf ("%s: error opening %s\n", argv[0], nout);
			}
			io_fclose (inf);
		} else {
			printf ("%s: error opening %s\n", argv[0], outfn);
		}
		xfree (nout);
	}

	exit (EXIT_SUCCESS);
}


/* caf_base64.c ends here */
