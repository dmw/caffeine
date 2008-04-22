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
#include "caf/caf_data_lstdl.h"
#include "caf/caf_dsm.h"

static caf_dsm_return_t *call_1 (void *s_data, caf_dsm_return_t *s_return);
static caf_dsm_return_t *call_2 (void *s_data, caf_dsm_return_t *s_return);
static caf_dsm_return_t *call_3 (void *s_data, caf_dsm_return_t *s_return);
static caf_dsm_return_t *call_4 (void *s_data, caf_dsm_return_t *s_return);
static caf_dsm_return_t *call_5 (void *s_data, caf_dsm_return_t *s_return);
static CAD_DSM_CALL(call_error);

static int d1 = 1;

caf_dsm_t *m1;

caf_dsm_state_t *s1;
caf_dsm_state_t *s2;
caf_dsm_state_t *s3;
caf_dsm_state_t *s4;
caf_dsm_state_t *s5;

caf_dsm_runner_t *runner;

static void init_states ();
static void init_dsm ();
static void delete_dsm ();
static void init_runner ();
static void delete_runner ();


int
main () {
	init_states ();
	init_dsm ();
	init_runner ();
	delete_runner ();
	delete_dsm ();
}


static void
init_states () {
	char msg[] =
	    "init_states():\n"
	    "\ts1 = %p\n"
	    "\ts2 = %p\n"
	    "\ts3 = %p\n"
	    "\ts4 = %p\n"
	    "\ts5 = %p\n";
	s1 = caf_dsm_state_new (1, CAF_DSM_STATE_START, call_1, call_error);
	s2 = caf_dsm_state_new (2, CAF_DSM_STATE_NORMAL, call_2, call_error);
	s3 = caf_dsm_state_new (3, CAF_DSM_STATE_NORMAL, call_3, call_error);
	s4 = caf_dsm_state_new (4, CAF_DSM_STATE_NORMAL, call_4, call_error);
	s5 = caf_dsm_state_new (5, CAF_DSM_STATE_END, call_5, call_error);
	printf (">>>----------------------------------------------------------\n");
	printf (msg, s1, s2, s3, s4, s5);
	printf ("<<<----------------------------------------------------------\n");
}


static void
init_dsm () {
	char msg[] =
	    "init_dsm ():\n"
	    "\tm1 = %p\n";
	m1 = caf_dsm_new (1, CAF_ERROR);
	if (m1 != (caf_dsm_t *)NULL) {
		caf_dsm_add (m1, s1);
		caf_dsm_add (m1, s2);
		caf_dsm_add (m1, s3);
		caf_dsm_add (m1, s4);
		caf_dsm_add (m1, s5);
	}
	printf (">>>----------------------------------------------------------\n");
	printf (msg, m1);
	lstdl_dump_ptr (stdout, m1->m_state);
	printf ("<<<----------------------------------------------------------\n");
}


static void
delete_dsm () {
	char msg[] =
	    "delete_dsm ():\n";
	caf_dsm_delete (m1);
	printf (">>>----------------------------------------------------------\n");
	printf (msg, m1);
	printf ("<<<----------------------------------------------------------\n");
}


static void
init_runner () {
	char msg[] =
	    "init_runner ():\n"
	    "\tm1 = %p\n"
	    "\trunner = %p\n";
	if (m1 != (caf_dsm_t *)NULL) {
		runner = caf_dsm_runner_new (m1, 1, CAF_DSM_CONTROL_FORWARD);
		printf (msg, m1, runner);
		d1 = 0;
		printf ("\trunner = %p && d1 = %p\n", (void *)runner, (void *)&d1);
		caf_dsm_runnner_work (runner, (void *)&d1);
	}
}


static void
delete_runner () {
	if (runner != (caf_dsm_runner_t *)NULL) {
		caf_dsm_runner_delete (runner);
	}
}


static caf_dsm_return_t *
call_1 (void *s_data, caf_dsm_return_t *s_return) {
	int *ld1;
	caf_dsm_return_t *r = (caf_dsm_return_t *)xmalloc (CAF_DSM_RETURN_SZ);
	if (r != (caf_dsm_return_t *)NULL && s_data != (void *)NULL) {
		s_return = r;
		ld1 = (int *)s_data;
		*ld1 = 1;
		r->r_data = s_data;
		r->r_return = s_data;
		r->r_control = CAF_DSM_CONTROL_FORWARD;
		printf ("call_1 (): %d [%p == %p]\n", *ld1, (void *)ld1, s_data);
		printf ("\treturns:\n\tcontrol = %p;\n\tdata = %p;\n\treturn: %p;\n",
				(void *)&(r->r_control), r->r_data, r->r_return);
	}
	return r;
}


static caf_dsm_return_t *
call_2 (void *s_data, caf_dsm_return_t *s_return) {
	int *ld1;
	caf_dsm_return_t *r = s_return;
	if (s_data != (void *)NULL && s_return != (void *)NULL) {
		ld1 = (int *)s_data;
		*ld1 = 2;
		r->r_data = (void *)ld1;
		r->r_return = (void *)ld1;
		r->r_control = CAF_DSM_CONTROL_FORWARD;
		printf ("call_2 (): %d [%p == %p]\n", *ld1, (void *)ld1, s_data);
		printf ("\treturns:\n\tcontrol = %p;\n\tdata = %p;\n\treturn: %p;\n",
				(void *)&(r->r_control), r->r_data, r->r_return);
	}
	return r;
}


static caf_dsm_return_t *
call_3 (void *s_data, caf_dsm_return_t *s_return) {
	int *ld1;
	caf_dsm_return_t *r = s_return;
	if (s_data != (void *)NULL && s_return != (void *)NULL) {
		ld1 = (int *)s_data;
		if (*ld1 == 4) {
			r->r_control = CAF_DSM_CONTROL_FORWARD;
			(*ld1) = 5;
		} else {
			r->r_control = CAF_DSM_CONTROL_FORWARD;
			(*ld1) = 3;
		}
		r->r_data = (void *)ld1;
		r->r_return = (void *)ld1;
		printf ("call_3 (): %d [%p == %p]\n", *ld1, (void *)ld1, s_data);
		printf ("\treturns:\n\tcontrol = %p;\n\tdata = %p;\n\treturn: %p;\n",
				(void *)&(r->r_control), r->r_data, r->r_return);
	}
	return r;
}


static caf_dsm_return_t *
call_4 (void *s_data, caf_dsm_return_t *s_return) {
	int *ld1;
	caf_dsm_return_t *r = s_return;
	if (s_data != (void *)NULL && s_return != (void *)NULL) {
		ld1 = (int *)s_data;
		if (*ld1 == 3) {
			r->r_control = CAF_DSM_CONTROL_BACKWARD;
			(*ld1) = 4;
		} else {
			r->r_control = CAF_DSM_CONTROL_FORWARD;
			(*ld1) = 6;
		}
		r->r_data = (void *)ld1;
		r->r_return = (void *)ld1;
		printf ("call_4 (): %d [%p == %p]\n", *ld1, (void *)ld1, s_data);
		printf ("\treturns:\n\tcontrol = %p;\n\tdata = %p;\n\treturn: %p;\n",
				(void *)&(r->r_control), r->r_data, r->r_return);
	}
	return r;
}


static caf_dsm_return_t *
call_5 (void *s_data, caf_dsm_return_t *s_return) {
	int *ld1;
	caf_dsm_return_t *r = s_return;
	if (s_data != (void *)NULL && s_return != (void *)NULL) {
		ld1 = (int *)s_data;
		(*ld1) = 7;
		r->r_data = (void *)ld1;
		r->r_return = (void *)ld1;
		r->r_control = CAF_DSM_CONTROL_FORWARD;
		printf ("call_5 (): %d [%p == %p]\n", *ld1, (void *)ld1, s_data);
		printf ("\treturns:\n\tcontrol = %p;\n\tdata = %p;\n\treturn: %p;\n",
				(void *)&(r->r_control), r->r_data, r->r_return);
	}
	return r;
}

/* caf_dsm.c ends here */
