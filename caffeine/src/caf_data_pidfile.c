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

#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/file.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#include "caf/caf.h"
#include "caf/caf_data_conv.h"
#include "caf/caf_data_pidfile.h"


int
pidfile_create (const char *path, pid_t pid) {
	char pid_str[64];
	int fd = 0;
	ssize_t sz = 0;
	if ((int)pid == 0) {
		pid = getpid ();
	}
	memset ((void *)pid_str, (int)NULL, 64);
	sprintf (pid_str, "%d", pid);
	fd = open (path, O_WRONLY | O_CREAT);
	if (fd >= 0) {
		fchmod (fd, 0600);
		sz = write (fd, (const void *)pid_str, strlen(pid_str));
		flock (fd, LOCK_EX);
		close (fd);
		return (int)sz;
	} else {
		return 0;
	}
}


int
pidfile_destroy (const char *path) {
	int fd = -1;
	if (!access (path, F_OK)) {
		fd = open (path, O_RDONLY);
		if (fd > 0) {
			flock (fd, LOCK_UN);
			close (fd);
		}
		return unlink (path);
	}
	return CAF_ERROR;
}


pid_t
pidfile_getpid (const char *path) {
	int pid = 0;
	char pid_str[64];
	int fd = 0;
	int old_errno = 0;
	ssize_t sz = 0;
	memset ((void *)pid_str, (int)NULL, 64);
	fd = open (path, O_RDONLY);
	if (fd > 0) {
		sz = read (fd, (void *)pid_str, 64);
		if (sz > 0) {
			old_errno = errno;
			errno = 0;
			pid = (pid_t)strtol (pid_str, (char **)NULL, 10);
			if (errno == 0) {
				errno = old_errno;
				close (fd);
				return pid;
			}
		}
		close (fd);
	}
	return 0;
}


pid_t
pidfile_getsid (const char *path) {
	pid_t pid = pidfile_getpid (path);
	if (pid > 0) {
		return getsid (pid);
	}
	return 0;
}


pid_t
pidfile_getpgid (const char *path) {
	pid_t pid = pidfile_getpid (path);
	if (pid > 0) {
		return getpgid (pid);
	}
	return 0;
}


int
pidfile_kill (const char *path, int sig) {
	int pid = pidfile_getpid (path);
	if (pid > 0 && sig >= 0) {
		return kill ((pid_t)pid, sig);
	}
	return CAF_ERROR;
}

/* caf_data_pidfile.c ends here */
