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
#ifndef CAF_SSM_H
#define CAF_SSM_H 1
/**
 * @defgroup      caf_dsm             Caffeine Static State Machine
 * @ingroup       caf_process
 * @addtogroup    caf_dsm
 * @{
 *
 * @brief     Caffeine Static State Machine
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine Static State Machine
 *
 */

#include <caf/caf.h>
#include <caf/caf_data_lstdl.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Caffeine SSM size */
#define CAF_SSM_SZ              (sizeof (caf_ssm_t))
/** Caffeine SSM State size */
#define CAF_SSM_STATE_SZ        (sizeof (caf_ssm_state_t))
/** Caffeine SSM Return size */
#define CAF_SSM_RETURN_SZ       (sizeof (caf_ssm_return_t))
/** Caffeine SSM Runner size */
#define CAF_SSM_RUNNER_SZ       (sizeof (caf_ssm_runner_t))
/** Caffeine SSM Callback */
#define CAD_SSM_CALL(call)      \
        caf_ssm_return_t *(*call)(void *s_data, caf_ssm_return_t *s_return)


/**
 * @brief		Caffeine SSM States
 *
 * Defines the state identifiers that can have the SSM
 * state list, the main use is the SSM control.
 *
 * @see		caf_ssm_state_t
 * @see		caf_ssm_state_s
 */
typedef enum {
	/** Startup state */
	CAF_SSM_STATE_START = 0000001,
	/** Normal state */
	CAF_SSM_STATE_NORMAL = 0000002,
	/** Error state */
	CAF_SSM_STATE_ERROR = 0000004,
	/** Ending state */
	CAF_SSM_STATE_END = 0000010
} caf_ssm_state_type_t;

/**
 * @brief		State return type
 *
 * This is the return type for a SSM, as the user defines the
 * different state interfaces, the return type holds the control
 * variable (r_control), the parameter data (r_data) and the
 * return value (r_return).
 *
 * @see			caf_ssm_return_s
 */
typedef struct caf_ssm_return_s caf_ssm_return_t;
/**
 * @brief		State return type
 *
 * This is the return type for a SSM, as the user defines the
 * different state interfaces, the return type holds the control
 * variable (r_control), the parameter data (r_data) and the
 * return value (r_return).
 *
 * @see			caf_ssm_return_t
 */
struct caf_ssm_return_s {
	/** Return Control, @see caf_ssm_state_control_t) */
	int r_control;
	/** Return Input Data */
	void *r_data;
	/** Return Data */
	void *r_return;
};

/**
 * @brief		SSM State Holder Typdef
 *
 * Holds the structure that contains the SSM state itself.
 * Has an identifier, a type a normal proceding callback and
 * an error callback.
 *
 * @see	caf_ssm_state_s
 */
typedef struct caf_ssm_state_s caf_ssm_state_t;
/**
 * @brief		SSM State Holder Typdef
 *
 * Holds the structure that contains the SSM state itself.
 * Has an identifier, a type a normal proceding callback and
 * an error callback.
 *
 * @see	caf_ssm_state_t
 */
struct caf_ssm_state_s {
	/** State identifier, must be unique in a SSM instance */
	int s_id;
	/** State type */
	int s_type;
	/** Proceding callback */
	CAD_SSM_CALL(s_call);
	/** Error callback */
	CAD_SSM_CALL(s_error);
};

/**
 *
 * @brief    Caffeine SSM storage type.
 * The type of a SSM storage structure.
 * @see      caf_ssm_s
 */
typedef struct caf_ssm_s caf_ssm_t;
/**
 *
 * @brief    Caffeine SSM structure
 * SSM structure, contains id, cycle flag, and machine
 * states list pointer.
 * @see      cbuffer_t
 */
struct caf_ssm_s {
	int m_id;
	int m_cycle;
	int last;
	int steps;
	caf_ssm_state_t **m_state;
};

/**
 * @brief		SSM State Control Types
 *
 * A SSM flow control can go forward, backward, stay or
 * jump to a error state. This defines the conduit that
 * follows the SSM.
 */
typedef enum {
	/** Go forward to the next state */
	CAF_SSM_CONTROL_FORWARD = 0000001,
	/** Go backward to the previous state */
	CAF_SSM_CONTROL_BACKWARD = 0000002,
	/** Stay in the current state */
	CAF_SSM_CONTROL_STAY = 0000004,
	/** Jump to an error condition */
	CAF_SSM_CONTROL_ERROR = 0000010
} caf_ssm_state_control_t;

/**
 * @brief		SSM Runner, this makes all SSMs thread safe
 *
 * A SSM runner it's a structure that holds the SSM instance
 * and the current state of it. Also, holds data about the
 * current call and many items that affects the SSM behavior.
 *
 * @see caf_ssm_runner_s
 */
typedef struct caf_ssm_runner_s caf_ssm_runner_t;
/**
 * @brief		SSM Runner, this makes all SSMs thread safe
 *
 * A SSM runner it's a structure that holds the SSM instance
 * and the current state of it. Also, holds data about the
 * current call and many items that affects the SSM behavior.
 *
 * @see caf_ssm_runner_t
 */
struct caf_ssm_runner_s {
	/** Runner identifier */
	int r_id;
	/** Runner State Control */
	int r_control;
	/** Runner SSM pointer */
	caf_ssm_t *r_machine;
	/** Runner State */
	caf_ssm_state_t *r_state;
	/** Runner State Callback */
	CAD_SSM_CALL(r_call);
	/** State Return Input Data */
	void *r_data;
	/** State Return Data */
	void *r_return;
	/** Current Node in State List */
	int l_current;
};

/**
 * @brief		Allocates a new SSM
 *
 * Allocates a new SSM (creates a new instance).
 *
 * @param id			SSM identifier
 * @param cycle			Cycle the SSM
 *
 * @return a new allocated caf_ssm_t pointer.
 */
caf_ssm_t *caf_ssm_new (int id, int cycle, int steps);

/**
 * @brief				Deallocates the given SSM
 *
 * Deallocates the given SSM.
 *
 * @param r				SSM to deallocate
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_ssm_delete (caf_ssm_t *r);

/**
 * @brief				Allocates a new State for the SSM
 *
 * Allocates a new State for a SSM.
 *
 * @param s_id			State identifier
 * @param s_type		State Type
 * @param s_call		State Call
 * @param s_error		State Error Callback
 *
 * @return A new allocated SSM state on success, NULL on failure.
 */
caf_ssm_state_t *caf_ssm_state_new (int s_id, caf_ssm_state_type_t s_type,
									CAD_SSM_CALL(s_call),
									CAD_SSM_CALL(s_error));

/**
 * @brief				Deallocates the given SSM state.
 *
 * Deallocates the given SSM state.
 *
 * @param r				SSM state to deallocate.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_ssm_state_delete (caf_ssm_state_t *r);

/**
 * @brief				Adds a state to the given SSM.
 *
 * Adds a new state or previouslly allocated state to
 * the given SSM, this allows you to repeat a state through
 * the State Running routine.
 *
 * @param m				SSM where to add the state.
 * @param s				State to add.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_ssm_add (caf_ssm_t *m, caf_ssm_state_t *s);

/**
 * @brief				Removes the given state from the SSM.
 *
 * Removes a State from the SSM. This allows you to remove
 * a part of the SSM.
 *
 * @param m				SSM where to remove from the given state.
 * @param s				State to add.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_ssm_remove (caf_ssm_t *m, caf_ssm_state_t *s);

/**
 * @brief				Creates a new Runner for the given SSM.
 *
 * Creates a new Runner for the given. A runner is a thread safe
 * running instance for a SSM. If you don't use shared static
 * variables or similar coding faults on your SSM, a runner is
 * usefull on thread safe tasks.
 *
 * @param m				SSM from where create a new Runner.
 * @param r_id			Runner identifier.
 * @param ctrl			Control Startup.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
caf_ssm_runner_t *caf_ssm_runner_new (caf_ssm_t *m, int r_id,
									  caf_ssm_state_control_t ctrl);

/**
 * @brief				Deallocates the given Runner.
 *
 * Deallocates the given Runner.
 *
 * @param r				Runner to deallocate.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_ssm_runner_delete (caf_ssm_runner_t *r);

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
int caf_ssm_runnner_work (caf_ssm_runner_t *r, void *r_data);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_SSM_H */
/* caf_ssm.h ends here */

