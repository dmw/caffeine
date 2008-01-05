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

/** Default priority for Asynchrnous I/O */
#define CAF_AIO_PRIORITY			0
/** Size of the caf_aio_file_t structure (@see caf_io_file_t) */
#define CAF_AIO_FILE_SZ				(sizeof (caf_aio_file_t))
/** Size of the aiocb (Asyncrhonous I/O Control Block) structure */
#define CAF_AIO_CTRLB_SZ			(sizeof (struct aiocb))
/** Compares the Buffer Size and Buffer I/O Size members of a
	cbuffer_t structure (@see cbuffer_t) */
#define CAF_CBUF_AIO_EQ(b)			((ssize_t)b->sz == b->iosz)
/** Cleans a caf_aio_file_t structure to the default values
	(@see caf_aio_file_t) */
#define CAF_AIO_CLEAN(f)								\
		f->iocb.aio_lio_opcode = LIO_NOP;				\
		f->iocb.aio_reqprio = CAF_AIO_PRIORITY;

/**
 * @brief Asynchrnous I/O File Type
 *
 * The type used to represent files in asynchronous I/O
 * operations.
 *
 * @see caf_aio_file_s
 */
typedef struct caf_aio_file_s caf_aio_file_t;

/**
 * @brief Asynchronous I/O File Representation
 *
 * This structure represents the an asynchronous opened file
 * containing all the data needed to operate with the file
 * descriptor.
 *
 * @see caf_aio_file_t
 */
struct caf_aio_file_s {
	/** File descriptor asociated with  */
	int fd;
	/** File <b>open(2)</b> flags  */
	int flags;
	/** File opened with <b>fstat(2)</b> handling */
	int ustat;
	/** File mode used with <b>open(2)</b> when created  */
	mode_t mode;
	/** File stats (used by <b>fstat(2)</b>) */
	struct stat sd;
	/** File path */
	char *path;
	/** File Asynchronous I/O Control Block */
	struct aiocb iocb;
	/** File Asynchronous Buffer */
	cbuffer_t *buf;
};

/**
 * @brief AIO Pool Control Type
 *
 * This type is used as control structure to work with
 * pools of AIO files, using batch operating system interfaces
 * to work on.
 *
 * @see caf_aio_file_lst_s
 */
typedef struct caf_aio_file_lst_s caf_aio_file_lst_t;

/**
 * @brief AIO Pool Control Structure
 *
 * This structure controls the AIO over pools of files
 * to work in batch operation modes, using the proper batch
 * system interfaces to work on AIO files.
 *
 * @see caf_aio_file_lst_t
 */
struct caf_aio_file_lst_s {
	/** Batch Open Flags  */
	int flags;
	/** Batch Open <b>fstat(2)</b> handling */
	int ustat;
	/** Batch <b>open(2)</b> mode when creating a files  */
	mode_t mode;
	/** Batch AIO Buffer size  */
	size_t iocb_sz;
	/** AIO Control Blocks Count  */
	int iocb_count;
	/** AIO File Paths  */
	char **iocb_paths;
	/** AIO File Descriptors  */
	int *iocb_fds;
	/** AIO Cache stats (<b>fstat(2)</b>)  */
	struct stat sd;
	/** AIO Control Block List (array) */
	struct aiocb *iocb_list;
};

/**
 * @brief Opens a file for a AIO
 *
 * @param path[in]				file path to open
 * @param flg[in]				open flags
 * @param md[in]				file mode on create files
 * @param fs[in]				retreive file stats (<b>fstat(2)</b>)
 * @param bsz[in]				file buffer size
 *
 * @return caf_aio_file_t *		an allocated aio file type
 *
 * @see caf_aio_file_t
 */
caf_aio_file_t *caf_aio_fopen (const char *path, const int flg,
							   const mode_t md, int fs, size_t bsz);

/**
 * @brief Close an opened file for AIO
 *
 * @param r[in]			pointer to the opened file
 *
 * @return int			CAF_OK on success, CAF_ERROR on failure
 */
int caf_aio_fclose (caf_aio_file_t *r);

/**
 * @brief
 *
 * @param r
 *
 * @return
 */
caf_aio_file_t *caf_aio_reopen (caf_aio_file_t *r);
int caf_aio_restat (caf_aio_file_t *r);
int caf_aio_fchanged (caf_aio_file_t *r, struct timespec *lmt,
					  struct timespec *lct);
int caf_aio_read (caf_aio_file_t *r, cbuffer_t *b);
int caf_aio_write (caf_aio_file_t *r, cbuffer_t *b);
int caf_aio_fcntl (caf_aio_file_t *r, int cmd, int *arg);
int caf_aio_flseek (caf_aio_file_t *r, off_t o, int w);
int caf_aio_cancel (caf_aio_file_t *r);
int caf_aio_return (caf_aio_file_t *r);
int caf_aio_error (caf_aio_file_t *r);
int caf_aio_suspend (caf_aio_file_t *r,
					 const struct timespec *to);
caf_aio_file_lst_t *caf_aio_lst_new (const int flg, const mode_t md,
									 int fs, int count);
int caf_aio_lst_delete (caf_aio_file_lst_t *r);
int caf_aio_lst_open (caf_aio_file_lst_t *r,
					  const char **paths);
int caf_aio_lst_close (caf_aio_file_lst_t *r);
int caf_aio_lst_suspend (caf_aio_file_lst_t *r,
						 const struct timespec *to, int idx);
int caf_aio_lst_operation (caf_aio_file_lst_t *r,
						   struct sigevent *e, int mode);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_AIO_CORE_H */
/* caf_aio_core.h ends here */

