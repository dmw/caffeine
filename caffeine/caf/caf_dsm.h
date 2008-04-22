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

  $Id$
*/
#ifndef CAF_DSM_H
#define CAF_DSM_H 1
/**
 * @defgroup      caf_dsm             Caffeine Dynamic State Machine
 * @ingroup       caf_process
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
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/** Caffeine DSM size */
#define CAF_DSM_SZ              (sizeof (caf_dsm_t))
/** Caffeine DSM State size */
#define CAF_DSM_STATE_SZ        (sizeof (caf_dsm_state_t))
/** Caffeine DSM Return size */
#define CAF_DSM_RETURN_SZ       (sizeof (caf_dsm_return_t))
/** Caffeine DSM Runner size */
#define CAF_DSM_RUNNER_SZ       (sizeof (caf_dsm_runner_t))
/** Caffeine DSM Callback */
#define CAD_DSM_CALL(call)      \
        caf_dsm_return_t *(*call)(void *s_data, caf_dsm_return_t *s_return)

/**
 *
 * @brief    Caffeine DSM storage type.
 * The type of a DSM storage structure.
 * @see      caf_dsm_s
 */
typedef struct caf_dsm_s caf_dsm_t;
/**
 *
 * @brief    Caffeine DSM structure
 * DSM structure, contains id, cycle flag, and machine
 * states list pointer.
 * @see      cbuffer_t
 */
struct caf_dsm_s {
	int m_id;
	int m_cycle;
	lstdl_t *m_state;
};

/**
 * @brief		Caffeine DSM States
 *
 * Defines the state identifiers that can have the DSM
 * state list, the main use is the DSM control.
 *
 * @see		caf_dsm_state_t
 * @see		caf_dsm_state_s
 */
typedef enum {
	/** Startup state */
	CAF_DSM_STATE_START = 0000001,
	/** Normal state */
	CAF_DSM_STATE_NORMAL = 0000002,
	/** Error state */
	CAF_DSM_STATE_ERROR = 0000004,
	/** Ending state */
	CAF_DSM_STATE_END = 0000010
} caf_dsm_state_type_t;

/**
 * @brief		State return type
 *
 * This is the return type for a DSM, as the user defines the
 * different state interfaces, the return type holds the control
 * variable (r_control), the parameter data (r_data) and the
 * return value (r_return).
 *
 * @see			caf_dsm_return_s
 */
typedef struct caf_dsm_return_s caf_dsm_return_t;
/**
 * @brief		State return type
 *
 * This is the return type for a DSM, as the user defines the
 * different state interfaces, the return type holds the control
 * variable (r_control), the parameter data (r_data) and the
 * return value (r_return).
 *
 * @see			caf_dsm_return_t
 */
struct caf_dsm_return_s {
	/** Return Control, @see caf_dsm_state_control_t) */
	int r_control;
	/** Return Input Data */
	void *r_data;
	/** Return Data */
	void *r_return;
};

/**
 * @brief		DSM State Holder Typdef
 *
 * Holds the structure that contains the DSM state itself.
 * Has an identifier, a type a normal proceding callback and
 * an error callback.
 *
 * @see	caf_dsm_state_s
 */
typedef struct caf_dsm_state_s caf_dsm_state_t;
/**
 * @brief		DSM State Holder Typdef
 *
 * Holds the structure that contains the DSM state itself.
 * Has an identifier, a type a normal proceding callback and
 * an error callback.
 *
 * @see	caf_dsm_state_t
 */
struct caf_dsm_state_s {
	/** State identifier, must be unique in a DSM instance */
	int s_id;
	/** State type */
	int s_type;
	/** Proceding callback */
	CAD_DSM_CALL(s_call);
	/** Error callback */
	CAD_DSM_CALL(s_error);
};

/**
 * @brief		DSM State Control Types
 *
 * A DSM flow control can go forward, backward, stay or
 * jump to a error state. This defines the conduit that
 * follows the DSM.
 */
typedef enum {
	/** Go forward to the next state */
	CAF_DSM_CONTROL_FORWARD = 0000001,
	/** Go backward to the previous state */
	CAF_DSM_CONTROL_BACKWARD = 0000002,
	/** Stay in the current state */
	CAF_DSM_CONTROL_STAY = 0000004,
	/** Jump to an error condition */
	CAF_DSM_CONTROL_ERROR = 0000010
} caf_dsm_state_control_t;

/**
 * @brief		DSM Runner, this makes all DSMs thread safe
 *
 * A DSM runner it's a structure that holds the DSM instance
 * and the current state of it. Also, holds data about the
 * current call and many items that affects the DSM behavior.
 *
 * @see caf_dsm_runner_s
 */
typedef struct caf_dsm_runner_s caf_dsm_runner_t;
/**
 * @brief		DSM Runner, this makes all DSMs thread safe
 *
 * A DSM runner it's a structure that holds the DSM instance
 * and the current state of it. Also, holds data about the
 * current call and many items that affects the DSM behavior.
 *
 * @see caf_dsm_runner_t
 */
struct caf_dsm_runner_s {
	/** Runner identifier */
	int r_id;
	/** Runner State Control */
	int r_control;
	/** Runner DSM pointer */
	caf_dsm_t *r_machine;
	/** Runner State */
	caf_dsm_state_t *r_state;
	/** Runner State Callback */
	CAD_DSM_CALL(r_call);
	/** State Return Input Data */
	void *r_data;
	/** State Return Data */
	void *r_return;
	/** Current Node in State List */
	lstdln_t *l_current;
};

/**
 * @brief		Allocates a new DSM
 *
 * Allocates a new DSM (creates a new instance).
 *
 * @param id			DSM identifier
 * @param cycle			Cycle the DSM
 *
 * @return a new allocated caf_dsm_t pointer.
 */
caf_dsm_t *caf_dsm_new (int id, int cycle);

/**
 * @brief				Deallocates the given DSM
 *
 * Deallocates the given DSM.
 *
 * @param r				DSM to deallocate
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_dsm_delete (caf_dsm_t *r);

/**
 * @brief				Allocates a new State for the DSM
 *
 * Allocates a new State for a DSM.
 *
 * @param s_id			State identifier
 * @param s_type		State Type
 * @param s_call		State Call
 * @param s_error		State Error Callback
 *
 * @return A new allocated DSM state on success, NULL on failure.
 */
caf_dsm_state_t *caf_dsm_state_new (int s_id, caf_dsm_state_type_t s_type,
									CAD_DSM_CALL(s_call),
									CAD_DSM_CALL(s_error));

/**
 * @brief				Deallocates the given DSM state.
 *
 * Deallocates the given DSM state.
 *
 * @param r				DSM state to deallocate.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_dsm_state_delete (caf_dsm_state_t *r);

/**
 * @brief				Adds a state to the given DSM.
 *
 * Adds a new state or previouslly allocated state to
 * the given DSM, this allows you to repeat a state through
 * the State Running routine.
 *
 * @param m				DSM where to add the state.
 * @param s				State to add.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_dsm_add (caf_dsm_t *m, caf_dsm_state_t *s);

/**
 * @brief				Removes the given state from the DSM.
 *
 * Removes a State from the DSM. This allows you to remove
 * a part of the DSM.
 *
 * @param m				DSM where to remove from the given state.
 * @param s				State to add.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_dsm_remove (caf_dsm_t *m, caf_dsm_state_t *s);

/**
 * @brief				Creates a new Runner for the given DSM.
 *
 * Creates a new Runner for the given. A runner is a thread safe
 * running instance for a DSM. If you don't use shared static
 * variables or similar coding faults on your DSM, a runner is
 * usefull on thread safe tasks.
 *
 * @param m				DSM from where create a new Runner.
 * @param r_id			Runner identifier.
 * @param ctrl			Control Startup.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
caf_dsm_runner_t *caf_dsm_runner_new (caf_dsm_t *m, int r_id,
									  caf_dsm_state_control_t ctrl);

/**
 * @brief				Deallocates the given Runner.
 *
 * Deallocates the given Runner.
 *
 * @param r				Runner to deallocate.
 *
 * @return CAF_OK on success, CAF_ERROR on failure.
 */
int caf_dsm_runner_delete (caf_dsm_runner_t *r);

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
int caf_dsm_runnner_work (caf_dsm_runner_t *r, void *r_data);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DSM_H */
/* caf_dsm.h ends here */

