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

  $Id$
*/
#ifndef CAF_PSM_H
#define CAF_PSM_H 1
/**
 * @defgroup      caf_dsm             Caffeine Plugable State Machine
 * @ingroup       caf_process
 * @addtogroup    caf_dsm
 * @{
 *
 * @brief     Caffeine Plugable State Machine
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine Plugable State Machine
 *
 */

#include <caf/caf.h>
#include <caf/caf_data_lstdl.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Caffeine PSM size */
#define CAF_PSM_SZ              (sizeof (caf_psm_t))
/** Caffeine PSM State size */
#define CAF_PSM_STATE_SZ        (sizeof (caf_psm_state_t))
/** Caffeine PSM Return size */
#define CAF_PSM_RETURN_SZ       (sizeof (caf_psm_return_t))
/** Caffeine PSM Runner size */
#define CAF_PSM_RUNNER_SZ       (sizeof (caf_psm_runner_t))
/** Caffeine PSM Callback */
#define CAD_PSM_CALL(call)      \
        caf_psm_return_t *(*call)(void *s_data, caf_psm_return_t *s_return)


/**
 * @brief		Caffeine PSM States
 *
 * Defines the state identifiers that can have the PSM
 * state list, the main use is the PSM control.
 *
 * @see		caf_psm_state_t
 * @see		caf_psm_state_s
 */
typedef enum {
	/** Startup state */
	CAF_PSM_STATE_START = 0000001,
	/** Normal state */
	CAF_PSM_STATE_NORMAL = 0000002,
	/** Error state */
	CAF_PSM_STATE_ERROR = 0000004,
	/** Ending state */
	CAF_PSM_STATE_END = 0000010
} caf_psm_state_type_t;

/**
 * @brief		State return type
 *
 * This is the return type for a PSM, as the user defines the
 * different state interfaces, the return type holds the control
 * variable (r_control), the parameter data (r_data) and the
 * return value (r_return).
 *
 * @see			caf_psm_return_s
 */
typedef struct caf_psm_return_s caf_psm_return_t;
/**
 * @brief		State return type
 *
 * This is the return type for a PSM, as the user defines the
 * different state interfaces, the return type holds the control
 * variable (r_control), the parameter data (r_data) and the
 * return value (r_return).
 *
 * @see			caf_psm_return_t
 */
struct caf_psm_return_s {
	/** Return Control, @see caf_psm_state_control_t) */
	int r_control;
	/** Return Input Data */
	void *r_data;
	/** Return Data */
	void *r_return;
};

/**
 * @brief		PSM State Holder Typdef
 *
 * Holds the structure that contains the PSM state itself.
 * Has an identifier, a type a normal proceding callback and
 * an error callback.
 *
 * @see	caf_psm_state_s
 */
typedef struct caf_psm_state_s caf_psm_state_t;
/**
 * @brief		PSM State Holder Typdef
 *
 * Holds the structure that contains the PSM state itself.
 * Has an identifier, a type a normal proceding callback and
 * an error callback.
 *
 * @see	caf_psm_state_t
 */
struct caf_psm_state_s {
	/** State identifier, must be unique in a PSM instance */
	int s_id;
	/** State type */
	int s_type;
	/** Proceding callback */
	CAD_PSM_CALL(s_call);
	/** Error callback */
	CAD_PSM_CALL(s_error);
};

/**
 *
 * @brief    Caffeine PSM storage type.
 * The type of a PSM storage structure.
 * @see      caf_psm_s
 */
typedef struct caf_psm_s caf_psm_t;
/**
 *
 * @brief    Caffeine PSM structure
 * PSM structure, contains id, cycle flag, and machine
 * states list pointer.
 * @see      cbuffer_t
 */
struct caf_psm_s {
	int m_id;
	int m_cycle;
	int last;
	int steps;
	caf_psm_state_t *m_state;
};

/**
 * @brief		PSM State Control Types
 *
 * A PSM flow control can go forward, backward, stay or
 * jump to a error state. This defines the conduit that
 * follows the PSM.
 */
typedef enum {
	/** Go forward to the next state */
	CAF_PSM_CONTROL_FORWARD = 0000001,
	/** Go backward to the previous state */
	CAF_PSM_CONTROL_BACKWARD = 0000002,
	/** Stay in the current state */
	CAF_PSM_CONTROL_STAY = 0000004,
	/** Jump to an error condition */
	CAF_PSM_CONTROL_ERROR = 0000010
} caf_psm_state_control_t;

/**
 * @brief		PSM Runner, this makes all PSMs thread safe
 *
 * A PSM runner it's a structure that holds the PSM instance
 * and the current state of it. Also, holds data about the
 * current call and many items that affects the PSM behavior.
 *
 * @see caf_psm_runner_s
 */
typedef struct caf_psm_runner_s caf_psm_runner_t;
/**
 * @brief		PSM Runner, this makes all PSMs thread safe
 *
 * A PSM runner it's a structure that holds the PSM instance
 * and the current state of it. Also, holds data about the
 * current call and many items that affects the PSM behavior.
 *
 * @see caf_psm_runner_t
 */
struct caf_psm_runner_s {
	/** Runner identifier */
	int r_id;
	/** Runner State Control */
	int r_control;
	/** Runner PSM pointer */
	caf_psm_t *r_machine;
	/** Runner State */
	caf_psm_state_t *r_state;
	/** Runner State Callback */
	CAD_PSM_CALL(r_call);
	/** State Return Input Data */
	void *r_data;
	/** State Return Data */
	void *r_return;
	/** Current Node in State List */
	int l_current;
};

/**
 * @brief		Allocates a new PSM
 *
 * Allocates a new PSM (creates a new instance).
 *
 * @param id			PSM identifier
 * @param cycle			Cycle the PSM
 *
 * @return a new allocated caf_psm_t pointer.
 */
caf_psm_t *caf_psm_new (int id, int cycle, int steps);

/**
 * @brief				Deallocates the given PSM
 *
 * Deallocates the given PSM.
 *
 * @param r				PSM to deallocate
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_psm_delete (caf_psm_t *r);

/**
 * @brief				Allocates a new State for the PSM
 *
 * Allocates a new State for a PSM.
 *
 * @param s_id			State identifier
 * @param s_type		State Type
 * @param s_call		State Call
 * @param s_error		State Error Callback
 *
 * @return A new allocated PSM state on success, NULL on failure.
 */
caf_psm_state_t *caf_psm_state_new (int s_id, caf_psm_state_type_t s_type,
									CAD_PSM_CALL(s_call),
									CAD_PSM_CALL(s_error));

/**
 * @brief				Deallocates the given PSM state.
 *
 * Deallocates the given PSM state.
 *
 * @param r				PSM state to deallocate.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_psm_state_delete (caf_psm_state_t *r);

/**
 * @brief				Adds a state to the given PSM.
 *
 * Adds a new state or previouslly allocated state to
 * the given PSM, this allows you to repeat a state through
 * the State Running routine.
 *
 * @param m				PSM where to add the state.
 * @param s				State to add.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_psm_add (caf_psm_t *m, caf_psm_state_t s);

/**
 * @brief				Removes the given state from the PSM.
 *
 * Removes a State from the PSM. This allows you to remove
 * a part of the PSM.
 *
 * @param m				PSM where to remove from the given state.
 * @param s				State to add.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_psm_remove (caf_psm_t *m, int s_id);

/**
 * @brief				Creates a new Runner for the given PSM.
 *
 * Creates a new Runner for the given. A runner is a thread safe
 * running instance for a PSM. If you don't use shared static
 * variables or similar coding faults on your PSM, a runner is
 * usefull on thread safe tasks.
 *
 * @param m				PSM from where create a new Runner.
 * @param r_id			Runner identifier.
 * @param ctrl			Control Startup.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
caf_psm_runner_t *caf_psm_runner_new (caf_psm_t *m, int r_id,
									  caf_psm_state_control_t ctrl);

/**
 * @brief				Deallocates the given Runner.
 *
 * Deallocates the given Runner.
 *
 * @param r				Runner to deallocate.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_psm_runner_delete (caf_psm_runner_t *r);

/**
 * @brief				Makes the runner work.
 *
 * Starts the runner, and the program/thread control passes to
 * the runner.
 *
 * @param r				Runner to work on.
 * @param r_data		Initial Runner Data.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_psm_runnner_work (caf_psm_runner_t *r, void *r_data);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_PSM_H */
/* caf_ssm.h ends here */

