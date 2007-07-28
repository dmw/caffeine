/* -*- mode: c; c-default-style: "k&r"; indent-tabs-mode: nil; -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 expandtab: */
/* kate: space-indent on; indent-width 4; mixedindent off; indent-mode none; */
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

    $Id$
 */
#ifndef CAF_DSM_H
#define CAF_DSM_H 1
/**
 * @defgroup      caf_dsm             Caffeine Dynamic State Machine
 * @ingroup       caf_data
 * @addtogroup    caf_dsm
 * @{
 *
 * @brief     Caffeine Dynamic State Machine
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine Dynamic State Machine
 *
 */

#include <caf/caf.h>
#include <caf/caf_data_lstdl.h>

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#define CAF_DSM_SZ              (sizeof (caf_dsm_t))
#define CAF_DSM_STATE_SZ        (sizeof (caf_dsm_state_t))
#define CAF_DSM_RETURN_SZ       (sizeof (caf_dsm_return_t))
#define CAF_DSM_RUNNER_SZ       (sizeof (caf_dsm_runner_t))
#define CAD_DSM_CALL(call)      \
        caf_dsm_return_t *(*call)(void *s_data, caf_dsm_return_t *s_return)

	typedef struct caf_dsm_s caf_dsm_t;
	struct caf_dsm_s {
		int m_id;
		int m_cycle;
		lstdl_t *m_state;
	};

	typedef enum {
	    CAF_DSM_STATE_START = 0000001,
	    CAF_DSM_STATE_NORMAL = 0000002,
	    CAF_DSM_STATE_ERROR = 0000004,
	    CAF_DSM_STATE_END = 0000010
	} caf_dsm_state_type_t;

	typedef struct caf_dsm_return_s caf_dsm_return_t;
	struct caf_dsm_return_s {
		                     int r_control;
		                     void *r_data;
		                     void *r_return;
	                     };

	typedef struct caf_dsm_state_s caf_dsm_state_t;
	struct caf_dsm_state_s {
		int s_id;
		int s_type;
		CAD_DSM_CALL(s_call);
		CAD_DSM_CALL(s_error);
	};

	typedef enum {
	    CAF_DSM_CONTROL_FORWARD = 0000001,
	    CAF_DSM_CONTROL_BACKWARD = 0000002,
	    CAF_DSM_CONTROL_STAY = 0000004,
	    CAF_DSM_CONTROL_ERROR = 0000010
	} caf_dsm_state_control_t;

	typedef struct caf_dsm_runner_s caf_dsm_runner_t;
	struct caf_dsm_runner_s {
		int r_id;
		int r_control;
		caf_dsm_t *r_machine;
		caf_dsm_state_t *r_state;
		CAD_DSM_CALL(r_call);
		void *r_data;
		void *r_return;
		lstdln_t *l_current;
	};

	caf_dsm_t *caf_dsm_new (int id, int cycle);
	int caf_dsm_delete (caf_dsm_t *r);
	caf_dsm_state_t *caf_dsm_state_new (int s_id, caf_dsm_state_type_t s_type,
	                                    CAD_DSM_CALL(s_call),
	                                    CAD_DSM_CALL(s_error));
	int caf_dsm_state_delete (caf_dsm_state_t *r);
	int caf_dsm_add (caf_dsm_t *m, caf_dsm_state_t *s);
	int caf_dsm_remove (caf_dsm_t *m, caf_dsm_state_t *s);
	caf_dsm_runner_t *caf_dsm_runner_new (caf_dsm_t *m, int r_id,
	                                      caf_dsm_state_control_t ctrl);
	int caf_dsm_runner_delete (caf_dsm_runner_t *r);
	int caf_dsm_runnner_work (caf_dsm_runner_t *r, void *r_data);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_BITS_H */
/* caf_dsm.h ends here */

