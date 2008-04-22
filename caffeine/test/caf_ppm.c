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

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "caf/caf_tool_macro.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_pidfile.h"
#include "caf/caf_process_pool.h"


int test_pt (void *p);
void test_signal_handler (int sig);

lstdl_t *pool;
int exit_success;

int
main (void) {
	extern lstdl_t *pool;
	extern int exit_success;
	exit_success = 0;

	pool = ppm_pool_create (3, (lstdl_t *)NULL, test_pt);
	sleep (20);
	ppm_kill_pool (pool, SIGHUP);
	lstdl_delete (pool, lstdl_delete_cb);
	return 0;
}

int
test_pt (void *p) {
	extern lstdl_t *pool;
	extern int exit_success;
	signal (SIGHUP, test_signal_handler);
	while (1 && exit_success == 0) {
		sleep (1);
		printf ("pid: %d (%p)\n", getpid (), p);
	}
	lstdl_delete (pool, lstdl_delete_cb);
	return 1;
}

void
test_signal_handler (int sig) {
	extern int exit_success;
	exit_success = 1;
	printf ("received signal: %d\n", sig);
}

/* caf_ppm.c ends here */
