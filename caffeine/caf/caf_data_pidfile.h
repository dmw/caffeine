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
#ifndef CAF_DATA_PIDFILE_H
#define CAF_DATA_PIDFILE_H 1

#include <sys/types.h>
#include <unistd.h>

/**
 * @defgroup      caf_pidfile    PID File Manager
 * @ingroup       caf_process
 * @addtogroup    caf_pidfile
 * @{
 *
 * @brief     Caffeine PID File Manager
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * PID File Manager for Caffeine. Allows create and desatroy PID Files for
 * Caffeine Applications.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define PIDFILE_NOPID               -1
#define PIDFILE_MASK                0000177

/**
 *
 * @brief    Creates a PID File.
 *
 * Creates a PID file in the given path. If the given PID is PIDFILE_NOPID,
 * the PID will be the PID that returns getpid(2).
 *
 * @param[in]    path            pid file path.
 * @param[in]    pid             pid to write on the file.
 * @return       int             the amount of bytes writen to pid file.
 *
 * @see      pidfile_destroy
 */
int pidfile_create (const char *path, pid_t pid);

/**
 *
 * @brief    Destroy (deletes) a PID File.
 *
 * Deletes a PID File on the given Path.
 *
 * @param[in]    path            pid file path.
 * @return       int             0 on success.
 *
 * @see      pidfile_create
 */
int pidfile_destroy (const char *path);

/**
 *
 * @brief    Gets the PID from a PID file.
 *
 * Gets the PID from a PID file. It's readed as int, but returned as
 * pid_t.
 *
 * @param[in]    path            pid file path.
 * @return       pid_t           the PID number.
 *
 * @see      pidfile_kill
 */
int pidfile_getpid (const char *path);

/**
 *
 * @brief    Gets the Parent PID from a PID file.
 *
 * Gets the Parent PID from a PID file. It's readed as int, but returned as
 * pid_t.
 *
 * @param[in]    path            pid file path.
 * @return       pid_t           the PID number.
 *
 * @see      pidfile_kill
 */
pid_t pidfile_getsid (const char *path);

/**
 *
 * @brief    Gets the Parent PID from a PID file.
 *
 * Gets the Parent PID from a PID file. It's readed as int, but returned as
 * pid_t.
 *
 * @param[in]    path            pid file path.
 * @return       pid_t           the PID number.
 *
 * @see      pidfile_kill
 */
pid_t pidfile_getpgid (const char *path);

/**
 *
 * @brief    Sends a SIGNAL to the PID in a PID file.
 *
 * Sends a SIGNAL to the PID in a PID file.
 *
 * @param[in]    path            pid file path.
 * @return       int             the kill(2) returning value.
 *
 * @see      pidfile_getpid
 */
int pidfile_kill (const char *path, int sig);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_PIDFILE_H */
/* caf_data_pidfile.h ends here */
