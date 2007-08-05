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
#ifndef CAF_PROCESS_POOL_H
#define CAF_PROCESS_POOL_H 1
/**
 * @defgroup      caf_process_pool    Process Pool Manager
 * @ingroup       caf_process
 * @addtogroup    caf_process_pool
 * @{
 *
 * @brief     Process Pool Manager.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Process Pool Manager is the set of functions to manage a process pool,
 * similar to other daemons that does multiple forks(2) at one time to
 * serve some task.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define CAF_ENTRY_POINT(pt)     int (*pt)(void *p)
#define CAF_PINFO_SZ            sizeof(proc_info_t)

#include <caf/caf_data_lstdl.h>

typedef struct proc_info_s proc_info_t;
struct proc_info_s {
	int number;
	pid_t pid;
	pid_t pgrp;
	lstdl_t *lst;
	CAF_ENTRY_POINT(point);
};


/**
 *
 * @brief    Creates a new Caffeine Process Information Structure.
 *
 * Allocates memory for a proc_info_t structure.
 *
 * @param[in]    num             process number.
 * @param[in]    pid             process pid.
 * @param[in]    pgrp            process group.
 * @param[in]    lst             parameter list to the entry point.
 * @param[in]    point           process entry point.
 * @return       proc_info_t *   the allocated proc_info_t structure.
 *
 * @see      proc_info_t
 */
proc_info_t *ppm_pinfo_new (int num, pid_t pid, pid_t pgrp, lstdl_t *lst,
							CAF_ENTRY_POINT(point));

/**
 *
 * @brief    Deallocates a proc_info_t structure pointer.
 *
 * Deallocates a proc_info_t structure pointer.
 *
 * @param[in]    nfo                the proc_info_t structure pointer.
 *
 * @see      proc_info_t
 */
void ppm_info_delete (proc_info_t *nfo);

/**
 *
 * @brief    Daemonize the current process.
 *
 * Daemonize the current process, asigning the process number, process group
 * and the list of parameters for the process entry point.
 *
 * @param[in]    num                process number.
 * @param[in]    pgrp               process group.
 * @param[in]    lst                parameter list.
 * @param[in]    pt                 process entry point.
 * @return       proc_info_t *      the allocated proc_info_t structure.
 *
 * @see      proc_info_t
 */
proc_info_t *ppm_daemonize (int num, pid_t pgrp, lstdl_t *,
							CAF_ENTRY_POINT(pt));

/**
 *
 * @brief    Sends signals to the pid in the given proc_info_t structure.
 *
 * Sends the given signal to the pid in the given proc_info_t structure.
 *
 * @param[in]    nfo                proc_info_t structure.
 * @param[in]    sig                signal to send.
 * @return       int                the kill result.
 *
 * @see      proc_info_t
 */
int ppm_kill (proc_info_t *nfo, int sig);

/**
 *
 * @brief    Sends a signal to the current process group.
 *
 * Sends a signal to the current process group
 *
 * @param[in]    sig                signal to send.
 * @return       int                the result of killpg.
 *
 */
int ppm_kill_group (int sig);

/**
 *
 * @brief    Creates a Pool or DLL with daemonized proc_info_t structures.
 *
 * Daemonize the c number of process and allocates for each of them a
 * proc_info_t structure, pushing a DLL with the allocated structures.
 * In the main process the list is returned, and the child process
 * executes the entry point with plst parameters.
 *
 * @param[in]    c                  process count to daemonize.
 * @param[in]    plst               parameters for process entry point.
 * @param[in]    pt                 process entry point.
 * @return       lstdl_t            the proc_info_t list.
 *
 * @see      proc_info_t
 * @see      lstdl_t
 */
lstdl_t *ppm_pool_create (int c, lstdl_t *plst, CAF_ENTRY_POINT(pt));

/**
 *
 * @brief    Creates a Pool or DLL with daemonized proc_info_t structures.
 *
 * Daemonize the c number of process and allocates for each of them a
 * proc_info_t structure, pushing a DLL with the allocated structures.
 * In the main process the list is returned, and the child process
 * executes the entry point with plst parameters.
 *
 * @param[in]    c                  process count to daemonize.
 * @param[in]    plst               parameters for process entry point.
 * @param[in]    pt                 process entry point.
 * @return       lstdl_t            the proc_info_t list.
 *
 * @see      proc_info_t
 * @see      lstdl_t
 */
int ppm_kill_pool (lstdl_t *lst, int sig);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_PROCESS_POOL_H */
/* caf_process_pool.h ends here */
