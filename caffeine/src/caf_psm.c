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
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstdl.h"
#include "caf/caf_psm.h"


caf_psm_t *
caf_psm_new (int id, int cycle, int steps) {
	caf_psm_t *r = (caf_psm_t *)NULL;
	caf_psm_state_t *s;
	int c;
	if (id > 0 && steps > 0) {
		r = (caf_psm_t *)xmalloc (CAF_PSM_SZ);
		if (r != (caf_psm_t *)NULL) {
			r->m_state = (caf_psm_state_t *)xmalloc ((size_t)steps * (size_t)(sizeof(NULL)));
			if (r->m_state != (caf_psm_state_t *)NULL) {
				for (c = 0; c < steps; c++) {
					s = &(r->m_state[c]);
					s->s_id = 0;
					s->s_type = 0;
					s->s_call = NULL;
					s->s_error = NULL;
				}
				r->m_id = id;
				r->m_cycle = cycle;
				r->last = 0;
				r->steps = steps;
			} else {
				xfree (r);
				r = (caf_psm_t *)NULL;
			}
		}
	}
	return r;
}


int
caf_psm_delete (caf_psm_t *r) {
	int c;
	caf_psm_state_t *s;
	if (r != (caf_psm_t *)NULL) {
		if (r->m_state != (caf_psm_state_t *)NULL) {
			for (c = 0; c < r->steps; c++) {
				s = &(r->m_state[c]);
				if (s->s_id != 0 && s->s_type != 0) {
					s->s_id = 0;
					s->s_type = 0;
					s->s_call = NULL;
					s->s_error = NULL;
				}
			}
			xfree (r->m_state);
			xfree (r);
			r = (caf_psm_t *)NULL;
			return CAF_OK;
		} else {
			xfree (r);
			r = (caf_psm_t *)NULL;
		}
	}
	return CAF_ERROR;
}


caf_psm_state_t *
caf_psm_state_new (int s_id, caf_psm_state_type_t s_type, CAD_PSM_CALL(s_call),
                   CAD_PSM_CALL(s_error)) {
	caf_psm_state_t *r = (caf_psm_state_t *)NULL;
	if (s_id > 0 && s_call != (void *)NULL) {
		r = (caf_psm_state_t *)xmalloc (CAF_PSM_STATE_SZ);
		if (r != (caf_psm_state_t *)NULL) {
			r->s_id = s_id;
			r->s_type = s_type;
			r->s_call = s_call;
			r->s_error = s_error;
			return r;
		}
	}
	return r;
}


int
caf_psm_state_delete (caf_psm_state_t *r) {
	if (r != (caf_psm_state_t *)NULL) {
		xfree (r);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_psm_add (caf_psm_t *m, caf_psm_state_t s) {
	if (m != (caf_psm_t *)NULL && s.s_id != 0 && s.s_type != 0) {
		m->last++;
		m->m_state[m->last] = s;
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_psm_remove (caf_psm_t *m, int s_id) {
	int c;
	caf_psm_state_t *s;
	if (s != (caf_psm_state_t *)NULL && m != (caf_psm_t *)NULL) {
		for (c = 0; c <= m->last; c++) {
			s = &(m->m_state[c]);
			if (s->s_id == s_id) {
				s->s_id = 0;
				s->s_type = 0;
				s->s_call = NULL;
				s->s_error = NULL;
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


caf_psm_runner_t *
caf_psm_runner_new (caf_psm_t *m, int r_id, caf_psm_state_control_t ctrl) {
	caf_psm_runner_t *r = (caf_psm_runner_t *)NULL;
	if (m != (caf_psm_t *)NULL && r_id > 0) {
		r = (caf_psm_runner_t *)xmalloc (CAF_PSM_RUNNER_SZ);
		if (r != (caf_psm_runner_t *)NULL) {
			r->r_id = r_id;
			r->r_machine = m;
			r->r_control = ctrl;
			r->r_state = (caf_psm_state_t *)NULL;
			r->r_call = (void *)NULL;
			r->r_data = (void *)NULL;
			r->r_return = (void *)NULL;
			r->l_current = 0;
			return r;
		}
	}
	return r;
}


int
caf_psm_runner_delete (caf_psm_runner_t *r) {
	if (r != (caf_psm_runner_t *)NULL) {
		xfree (r);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_psm_runnner_work (caf_psm_runner_t *r, void *r_data) {
	caf_psm_t *m = (caf_psm_t *)NULL;
	caf_psm_state_t *s = (caf_psm_state_t *)NULL;
	caf_psm_return_t *rt = (caf_psm_return_t *)NULL;
	int c, l;
	int passed = CAF_ERROR;
	if (r != (caf_psm_runner_t *)NULL) {
		m = r->r_machine;
		if (m != (caf_psm_t *)NULL) {
			if (m->m_state != (caf_psm_state_t *)NULL) {
				for (c = 0; c <= m->last; c++) {
					s = &(m->m_state[c]);
					if (s->s_call != NULL) {
						if (passed != CAF_OK) {
							rt = (caf_psm_return_t *)s->s_call (r_data,
																(void *)NULL);
							passed = CAF_OK;
						} else {
							rt = (caf_psm_return_t *)s->s_call (r->r_data,
																r->r_return);
						}
						r->r_return = rt;
						r->r_call = s->s_call;
						r->l_current = c;
						r->r_data = rt->r_data;
						r->r_control = rt->r_control;
						switch (r->r_control) {
						case CAF_PSM_CONTROL_FORWARD:
							break;
						case CAF_PSM_CONTROL_BACKWARD:
							l = c - 2;
							if (l > -1 && m->m_state[l].s_id != 0 &&
								m->m_state[l].s_type != 0) {
								c = l;
								s = &(m->m_state[l]);
							}
							break;
						case CAF_PSM_CONTROL_STAY:
							l = c - 1;
							if (l > -1 && m->m_state[l].s_id != 0 &&
								m->m_state[l].s_type != 0) {
								c = l;
								s = &(m->m_state[l]);
							}
							break;
						case CAF_PSM_CONTROL_ERROR:
							if (s->s_error != NULL) {
								rt = s->s_error (rt->r_data, rt->r_return);
								r->r_call = s->s_error;
								r->l_current = c;
								l = c - 2;
								if (l > -1 && m->m_state[l].s_id != 0 &&
									m->m_state[l].s_type != 0) {
									s = &(m->m_state[l]);
									c = l;
									r->l_current = c;
								}
							}
							break;
						default:
							return CAF_ERROR;
						}
					}
				}
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


/* caf_ssm.c ends here */

