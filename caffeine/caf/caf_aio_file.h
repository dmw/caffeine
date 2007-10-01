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
#ifndef CAF_AIO_CORE_H
#define CAF_AIO_CORE_H 1

#include <sys/types.h>
#include <sys/stat.h>
#include <aio.h>

/**
 * @defgroup      caf_aio_core           Core Asynchronous I/O
 * @ingroup       caf_io
 * @addtogroup    caf_aio_core
 * @{
 *
 * @brief     Core Asynchronous I/O
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Core Asynchronous I/O
 *
 */

#include <caf/caf_data_buffer.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

#define CAF_AIO_PRIORITY			0
#define CAF_AIO_FILE_SZ				(sizeof (caf_aio_file_t))
#define CAF_CBUF_AIO_EQ(b)			((ssize_t)b->sz == b->iosz)
#define CAF_AIO_CLEAN(f)			\
		f->iocb.aio_lio_opcode = LIO_NOP;				\
		f->iocb.aio_reqprio = CAF_AIO_PRIORITY;

typedef struct caf_aio_file_s caf_aio_file_t;
struct caf_aio_file_s {
	int fd;
	int flags;
	int ustat;
	mode_t mode;
	struct stat sd;
	char *path;
	struct aiocb iocb;
	cbuffer_t *buf;
};

caf_aio_file_t *caf_aio_fopen(const char *path, const int flg,
							  const mode_t md, int fs, size_t bsz);
int caf_aio_fclose(caf_aio_file_t *r);
caf_aio_file_t *caf_aio_reopen(caf_aio_file_t *r);
int caf_aio_restat(caf_aio_file_t *r);
int caf_aio_fchanged(caf_aio_file_t *r, struct timespec *lmt,
					 struct timespec *lct);
int caf_aio_read(caf_aio_file_t *r, cbuffer_t *b);
int caf_aio_write(caf_aio_file_t *r, cbuffer_t *b);
int caf_aio_fcntl(caf_aio_file_t *r, int cmd, int *arg);
int caf_aio_flseek(caf_aio_file_t *r, off_t o, int w);
int caf_aio_cancel(caf_aio_file_t *r);
int caf_aio_return(caf_aio_file_t *r);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_AIO_CORE_H */
/* caf_aio_core.h ends here */
