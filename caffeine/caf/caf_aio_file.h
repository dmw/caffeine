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
 * @author    Daniel Molina Wegener <dmw@coder.cl>
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

/** <b>caf_aio_cancel</b> return value when cancelation is done */
#define CAF_AIO_CANCELED			0x0010
/** <b>caf_aio_cancel</b> return value when cancelation is not done */
#define CAF_AIO_NOTCANCELED			0x0020
/** <b>caf_aio_cancel</b> return value when AIO operations are done
    and can not be canceled */
#define CAF_AIO_ALLDONE				0x0030
/** <b>caf_aio_cancel</b> return value when a wrong
	<b>caf_aio_file_t</b> parameter is passed */
#define CAF_AIO_CANCELBADF			0x0040

/** <b>caf_aio_read</b> operation flag */
#define CAF_AIO_READ			0x0010
/** <b>caf_aio_write</b> operation flag */
#define CAF_AIO_WRITE			0x0020
/** <b>caf_aio_fsync</b> operation flag */
#define CAF_AIO_FSYNC			0x0030
/** <b>caf_aio_restat</b> operation flag */
#define CAF_AIO_FCNTL			0x0040
/** <b>caf_aio_fcntl</b> operation flag */
#define CAF_AIO_LSEEK			0x0050
/** <b>caf_aio_lseek</b> operation flag */
#define CAF_AIO_CANCEL			0x0060
/** <b>caf_aio_cancel</b> operation flag */
#define CAF_AIO_RSTAT			0x0070
/** <b>caf_aio_restat</b> operation flag  */
#define CAF_AIO_NOOP			0x1000

/**
 * @brief Asynchrnous I/O File Type.
 *
 * The type used to represent files in asynchronous I/O
 * operations.
 *
 * @see caf_aio_file_s
 */
typedef struct caf_aio_file_s caf_aio_file_t;

/**
 * @brief Asynchronous I/O File Representation.
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
	/** <b>errno(3)</b> for AIO operations */
	int errno;
	/** Last AIO operation */
	int lastop;
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
 * @brief AIO Pool Control Type.
 *
 * This type is used as control structure to work with
 * pools of AIO files, using batch operating system interfaces
 * to work on.
 *
 * @see caf_aio_file_lst_s
 */
typedef struct caf_aio_file_lst_s caf_aio_file_lst_t;

/**
 * @brief AIO Pool Control Structure.
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
 * @brief Opens a file for AIO operations.
 *
 * Opens a file for asynchronous IO operations, checking every
 * step in the process of opening the file and setting the
 * proper data to operate with these files. The arguments
 * to this functions are the <b>path</b> to the file
 * to open, the argument <b>flg</b> are the flags of
 * the file to <b>open(2)</b>, <b>md</b> are the opening
 * mode flags, <b>fs</b> when is <b>CAF_OK</b> retreives
 * the file stats with <b>fstat(2)</b> and <b>bsz</b>
 * sets the file buffer size using <b>caf_buffer_t</b>.
 *
 * @param path[in]				path to the to open
 * @param flg[in]				open flags
 * @param md[in]				file modes on file creation
 * @param fs[in]				retreive file stats (<b>fstat(2)</b>)
 * @param bsz[in]				file buffer size
 *
 * @return caf_aio_file_t *		an allocated aio file
 *
 * @see caf_aio_file_t
 * @see caf_buffer_t
 */
caf_aio_file_t *caf_aio_fopen (const char *path, const int flg,
							   const mode_t md, int fs, size_t bsz);

/**
 * @brief Close an opened file for AIO
 *
 * Closes the file descriptor asociated with the file
 * and releases the memory asociated with the given
 * <b>caf_aio_file_t</b> structure <b>r</b>.
 *
 * @param r[in]			pointer to the opened file
 *
 * @return int			CAF_OK on success, CAF_ERROR on failure
 *
 * @see caf_aio_file_t
 */
int caf_aio_fclose (caf_aio_file_t *r);

/** 
 * @brief Repeat Open operation.
 *
 * Repeats the open operation over the given <b>caf_aio_file_t</b>
 * structure <b>r</b>, allocating a new pointer with <b>r</b> data
 * and using <b>caf_aio_file_open</b> to allocate it, also uses
 * <b>caf_aio_fclose</b> to clear the old pointer references.
 * 
 * @param r[in]			pointer to the opened file to reopen
 * 
 * @return caf_aio_file_t *		new allocated pointer or NULL on failure
 */
caf_aio_file_t *caf_aio_reopen (caf_aio_file_t *r);

/** 
 * @brief Refills the file stats
 * 
 * Calls <b>fstat(2)</b> over the asociated file descriptor and
 * restores file stats with fresh data.
 * 
 * @param r[in]			pointer to the file to restat
 * 
 * @return int			CAF_OK on success, CAF_ERROR on failure.
 */
int caf_aio_restat (caf_aio_file_t *r);

/** 
 * @brief Checks if the file has changed
 *
 * Checks if the has changed using the file stats timestamps,
 * against the given timestamps (<b>lmt</b> and <b>lct</b>).
 * The behaviour of this functions depends on the system
 * implementation of <b>fstat(2)</b> and <b>clock_gettime(2)</b>.
 *
 * @param r[in]			file to check
 * @param lmt[in]		modification timespec to check against
 * @param lct[in]		change timespec to check against
 * 
 * @return				CAF_OK with changes, CAF_ERROR with no changes.
 */
int caf_aio_fchanged (caf_aio_file_t *r, struct timespec *lmt,
					  struct timespec *lct);

/** 
 * @brief Reads asynchrnously from the given file
 *
 * Uses the standard <b>aio_read(2)</b> to read from the given
 * <b>caf_aio_file_t</b> structure <b>r</b> and using the
 * <b>cbuffer_t</b> structure <b>b</b> the data readed from
 * the <b>r</b> asociated file descriptor. Be carefull with the
 * <b>b</b> buffer, since the read and write operations are
 * asynchrnous, you may use some kind of <i>resource locking</i>
 * technique.
 * 
 * @param r[in]			file to read from
 * @param b[in]			buffer to fill with data
 * 
 * @return int			the amount of readed bytes.
 */
int caf_aio_read (caf_aio_file_t *r, cbuffer_t *b);

/** 
 * @brief Writes asynchrnously from the given file
 *
 * Uses the standard <b>aio_write(2)</b> to write to the given
 * <b>caf_aio_file_t</b> structure <b>r</b> using the data
 * contained in the <b>cbuffer_t</b> structure <b>b</b>.
 * Be carefull with the <b>b</b> buffer, since the read and
 * write operations are asynchrnous, you may use some kind of
 * <i>resource locking</i> technique.
 * 
 * @param r[in]			file to read from
 * @param b[in]			buffer to fill with data
 * 
 * @return int			the amount of readed bytes.
 */
int caf_aio_write (caf_aio_file_t *r, cbuffer_t *b);

/** 
 * @brief Applies <b>fcntl(2)</b> to the given AIO file
 *
 * Applies <b>fcntl(2)</b> commands to the given
 * <b>caf_aio_file_t</b> structure file descriptor.
 * 
 * @param r[in]			the file where to apply the commands
 * @param cmd[in]		<b>fcntl(2)</b> command
 * @param arg[in]		<b>fcntl(2)</b> command argument
 * 
 * @return int			the results of the <b>fcntl(2)</b> operation.
 */
int caf_aio_fcntl (caf_aio_file_t *r, int cmd, int *arg);

/** 
 * @brief Apply <b>lseek(2)</b> to the given AIO file
 * 
 * Applies <b>lseek(2)</b> to the given <b>caf_aio_file_t</b>
 * structure <b>r</b>, using the offset (<b>off_t</b>) <b>o</b>
 * and the whence flags <b>w</b>.
 *
 * @param r[in]			the file to operate
 * @param o[in]			the wanted file offset
 * @param w[in]			whence flags
 * 
 * @return int			CAF_OK on success, CAF_ERROR on failure.
 */
int caf_aio_flseek (caf_aio_file_t *r, off_t o, int w);

/** 
 * @brief Try to cancel AIO operations
 *
 * Try to cancel AIO operations related to the file descriptor
 * asociated with the <b>caf_aio_file_t</b> parameter <b>r</b>.
 * Returns CAF_AIO_CANCELED, CAF_AIO_NOTCANCELED, CAF_AIO_ALLDONE
 * and CAF_AIO_CANCELBADF, according to the cancel operation
 * results and parameters given.
 * 
 * @param r[in]			file where to cancel operations
 * 
 * @return int			integer flag.
 *
 * @see CAF_AIO_CANCELED
 * @see CAF_AIO_NOTCANCELED
 * @see CAF_AIO_ALLDONE
 * @see CAF_AIO_CANCELBADF
 *
 */
int caf_aio_cancel (caf_aio_file_t *r);

/** 
 * @brief Applies <b>aio_return(2)</b> to the given file
 * 
 * Applies the <b>aio_return(2)</b> interface to the given
 * <b>caf_aio_file_t</b> parameter <b>r</b> asociated file
 * descriptor.
 *
 * @param r[in]			file where to apply the interface
 * 
 * @return int				operation result on success, -1 on failure.
 */
int caf_aio_return (caf_aio_file_t *r);

/** 
 * @brief Gives the status of the AIO operation
 * 
 * Gives the error status of the AIO operation by appliying
 * <b>aio_error(2)</b> to the asociated file descriptor in
 * the given <b>caf_aio_file_t</b> parameter <b>r</b>.
 *
 * @param r[in]			file where to apply the interface
 * 
 * @return int			zero on success, error value on error.
 */
int caf_aio_error (caf_aio_file_t *r);

/** 
 * @brief Wait for AIO operation completion.
 *
 * Suspends the calling process to wait for AIO operations on the
 * given file, applying <b>aio_suspend(2)</b> system call or waits
 * for timeout.
 * 
 * @param r[in]			file from where to wait for AIO
 * @param to[in]		timeout specs
 * 
 * @return int			zero on success, -1 on failure.
 */
int caf_aio_suspend (caf_aio_file_t *r,
					 const struct timespec *to);

/** 
 * @brief Prepares a file list for AIO operations
 *
 * Prepares file list of <b>caf_aio_file_t</a> for AIO
 * operations and stores it in a <b>caf_aio_file_lst_t</b>
 * with the common opening data such as flags and modes.
 * 
 * @param flg[in]		opening flags
 * @param md[in]		opening mode
 * @param fs[in]		flag to use stats
 * @param count[in]		file count
 * 
 * @return caf_aio_file_lst_t	list of files to operate
 */
caf_aio_file_lst_t *caf_aio_lst_new (const int flg, const mode_t md,
									 int fs, int count);

/** 
 * @brief Deallocates an AIO file list
 *
 * Deallocates an AIO file list (<b>caf_aio_file_lst_t</b>)
 * parameter <b>r</b>, but this interface does not <b>close</b>
 * the file descriptor asociated with the list.
 * 
 * @param r[in]			list to deallocate
 * 
 * @return int			CAF_OK on success, CAF_ERROR on failure
 */
int caf_aio_lst_delete (caf_aio_file_lst_t *r);

/** 
 * @brief Batch open the file list for AIO
 *
 * Batch open the files for AIO operations. 
 *
 * @param r[in]			list where to store the files
 * @param paths[in]		list of paths to open
 * 
 * @return 
 */
int caf_aio_lst_open (caf_aio_file_lst_t *r,
					  const char **paths);

/** 
 * 
 * 
 * @param r 
 * 
 * @return 
 */
int caf_aio_lst_close (caf_aio_file_lst_t *r);

/** 
 * 
 * 
 * @param r 
 * @param to 
 * @param idx 
 * 
 * @return 
 */
int caf_aio_lst_suspend (caf_aio_file_lst_t *r,
						 const struct timespec *to, int idx);

/** 
 * 
 * 
 * @param r 
 * @param e 
 * @param mode 
 * 
 * @return 
 */
int caf_aio_lst_operation (caf_aio_file_lst_t *r,
						   struct sigevent *e, int mode);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_AIO_CORE_H */
/* caf_aio_core.h ends here */

