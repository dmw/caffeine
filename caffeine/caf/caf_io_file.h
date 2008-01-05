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
#ifndef CAF_IO_CORE_H
#define CAF_IO_CORE_H 1

#include <sys/types.h>
#include <sys/stat.h>

/**
 * @defgroup      caf_io_core           Core I/O
 * @ingroup       caf_io
 * @addtogroup    caf_io_core
 * @{
 *
 * @brief     Core I/O
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Core I/O
 *
 */

#include <caf/caf_data_buffer.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define CAF_IO_FILE_SZ              (sizeof (caf_io_file_t))
#define CAF_CBUF_IO_EQ(b)           ((ssize_t)b->sz == b->iosz)

typedef struct caf_io_file_s caf_io_file_t;
struct caf_io_file_s {
	int fd;
	int flags;
	int ustat;
	mode_t mode;
	struct stat sd;
	char *path;
};

caf_io_file_t *io_fopen (const char *path, const int flg,
						 const mode_t md, int fs);
int io_fclose (caf_io_file_t *r);
caf_io_file_t *io_reopen (caf_io_file_t *r);
int io_restat (caf_io_file_t *r);
int io_fchanged (caf_io_file_t *r, struct timespec *lmt,
				 struct timespec *lct);
ssize_t io_read (caf_io_file_t *r, cbuffer_t *b);
ssize_t io_write (caf_io_file_t *r, cbuffer_t *b);
int io_pipe (caf_io_file_t *r);
int io_fcntl (caf_io_file_t *r, int cmd, int *arg);
int io_flseek (caf_io_file_t *r, off_t o, int w);
int io_can_open (const char *path, int oflg);
int io_check_stat_flags (const struct stat *sd, int flg);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_IO_CORE_H */
/* caf_io_file.h ends here */

