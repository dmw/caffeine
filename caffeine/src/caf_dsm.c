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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_lstdl.h"
#include "caf/caf_dsm.h"


static int caf_dsm_delete_callback (void *data);
static int caf_dsm_state_delete_callback (void *r);


caf_dsm_t *
caf_dsm_new (int id, int cycle) {
	caf_dsm_t *r = (caf_dsm_t *)NULL;
	if (id > 0) {
		r = (caf_dsm_t *)xmalloc (CAF_DSM_SZ);
		if (r != (caf_dsm_t *)NULL) {
			r->m_state = lstdl_create ();
			if (r->m_state != (lstdl_t *)NULL) {
				r->m_id = id;
				r->m_cycle = cycle;
			} else {
				xfree (r);
				r = (caf_dsm_t *)NULL;
			}
		}
	}
	return r;
}


int
caf_dsm_delete (caf_dsm_t *r) {
	if (r != (caf_dsm_t *)NULL) {
		if (r->m_state != (lstdl_t *)NULL) {
			if ((lstdl_delete (r->m_state, caf_dsm_delete_callback))
				== CAF_OK) {
				xfree (r);
				return CAF_OK;
			}
		} else {
			xfree (r);
			r = (caf_dsm_t *)NULL;
		}
	}
	return CAF_ERROR;
}


caf_dsm_state_t *
caf_dsm_state_new (int s_id, caf_dsm_state_type_t s_type, CAD_DSM_CALL(s_call),
                   CAD_DSM_CALL(s_error)) {
	caf_dsm_state_t *r = (caf_dsm_state_t *)NULL;
	if (s_id > 0 && s_call != (void *)NULL) {
		r = (caf_dsm_state_t *)xmalloc (CAF_DSM_STATE_SZ);
		if (r != (caf_dsm_state_t *)NULL) {
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
caf_dsm_state_delete (caf_dsm_state_t *r) {
	if (r != (caf_dsm_state_t *)NULL) {
		xfree (r);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_dsm_add (caf_dsm_t *m, caf_dsm_state_t *s) {
	if (s != (caf_dsm_state_t *)NULL && m != (caf_dsm_t *)NULL) {
		if ((lstdl_push (m->m_state, (void *)s)) != (lstdl_t *)NULL) {
			return CAF_OK;
		}
	}
	return CAF_ERROR;
}


int
caf_dsm_remove (caf_dsm_t *m, caf_dsm_state_t *s) {
	if (s != (caf_dsm_state_t *)NULL && m != (caf_dsm_t *)NULL) {
		return lstdl_node_delete_by_data (m->m_state, (void *)s,
		                                  caf_dsm_state_delete_callback);
	}
	return CAF_ERROR;
}


caf_dsm_runner_t *
caf_dsm_runner_new (caf_dsm_t *m, int r_id, caf_dsm_state_control_t ctrl) {
	caf_dsm_runner_t *r = (caf_dsm_runner_t *)NULL;
	if (m != (caf_dsm_t *)NULL && r_id > 0) {
		r = (caf_dsm_runner_t *)xmalloc (CAF_DSM_RUNNER_SZ);
		if (r != (caf_dsm_runner_t *)NULL) {
			r->r_id = r_id;
			r->r_machine = m;
			r->r_control = ctrl;
			r->r_state = (caf_dsm_state_t *)NULL;
			r->r_call = (void *)NULL;
			r->r_data = (void *)NULL;
			r->r_return = (void *)NULL;
			r->l_current = (lstdln_t *)NULL;
			return r;
		}
	}
	return r;
}


int
caf_dsm_runner_delete (caf_dsm_runner_t *r) {
	if (r != (caf_dsm_runner_t *)NULL) {
		xfree (r);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_dsm_runnner_work (caf_dsm_runner_t *r, void *r_data) {
	caf_dsm_t *m = (caf_dsm_t *)NULL;
	caf_dsm_state_t *s = (caf_dsm_state_t *)NULL;
	caf_dsm_return_t *rt = (caf_dsm_return_t *)NULL;
	lstdln_t *n = (lstdln_t *)NULL;
	int passed = CAF_ERROR;
	if (r != (caf_dsm_runner_t *)NULL) {
		m = r->r_machine;
		if (m != (caf_dsm_t *)NULL) {
			if (m->m_state != (lstdl_t *)NULL) {
				n = m->m_state->head;
				while (n != (lstdln_t *)NULL) {
					s = (caf_dsm_state_t *)n->data;
					if (passed != CAF_OK) {
						rt = (caf_dsm_return_t *)s->s_call (r_data,
						                                    (void *)NULL);
						passed = CAF_OK;
					} else {
						rt = (caf_dsm_return_t *)s->s_call (r->r_data,
						                                    r->r_return);
					}
					r->r_return = rt;
					r->r_call = s->s_call;
					r->l_current = n;
					r->r_data = rt->r_data;
					r->r_control = rt->r_control;
					switch (r->r_control) {
					case CAF_DSM_CONTROL_FORWARD:
						break;
					case CAF_DSM_CONTROL_BACKWARD:
						if (n->prev != (lstdln_t *)NULL) {
							n = n->prev;
							if (n->prev != (lstdln_t *)NULL) {
								n = n->prev;
							}
						}
						break;
					case CAF_DSM_CONTROL_STAY:
						if (n->prev != (lstdln_t *)NULL) {
							n = n->prev;
						}
						break;
					case CAF_DSM_CONTROL_ERROR:
						if (s->s_error != NULL) {
							rt = s->s_error (rt->r_data, rt->r_return);
							r->r_call = s->s_error;
							r->l_current = n;
							if (n->prev != (lstdln_t *)NULL) {
								n = n->prev;
								if (n->prev != (lstdln_t *)NULL) {
									n = n->prev;
								}
							}
						}
						break;
					default:
						return CAF_ERROR;
					}
					n = n->next;
				}
				return CAF_OK;
			}
		}
	}
	return CAF_ERROR;
}


static int
caf_dsm_delete_callback (void *data) {
	if (data != (void *)NULL) {
		xfree (data);
		return CAF_OK;
	}
	return CAF_ERROR;
}


static int
caf_dsm_state_delete_callback (void *r) {
	if (r != (void *)NULL) {
		xfree (r);
		return CAF_OK;
	}
	return CAF_ERROR;
}

/* caf_dsm.c ends here */
