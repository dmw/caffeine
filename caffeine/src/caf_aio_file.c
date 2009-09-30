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
*/
#ifndef lint
static char Id[] = "$Id$";
#endif /* !lint */

#ifdef HAVE_CONFIG_H
#include "caf/config.h"
#endif /* !HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef HAVE_AIO_H
#include <aio.h>
#else
#error We need aio.h
#endif /* !HAVE_AIO_H */

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_io_file.h"
#include "caf/caf_aio_file.h"


caf_aio_file_t *
caf_aio_fopen (const char *path, const u_int flg, const mode_t md,
			   int fs, size_t bsz) {
	int prev_errno;
	caf_aio_file_t *r = (caf_aio_file_t *)NULL;
	if (path == (char *)NULL) {
		return r;
	}
	r = (caf_aio_file_t *)xmalloc (CAF_IO_FILE_SZ);
	if (r != (caf_aio_file_t *)NULL) {
		prev_errno = errno;
		errno = 0;
		if (md != 0) {
			r->iocb.aio_fildes = open (path, flg, md);
		} else {
			r->iocb.aio_fildes = open (path, flg);
		}
		r->aio_errno = errno;
		errno = prev_errno;
		if (r->iocb.aio_fildes >= 0) {
			if (fs == CAF_OK) {
				r->flags = flg;
				r->mode = md;
				r->path = strdup (path);
				r->ustat = fs;
				r->aio_errno = 0;
				r->lastop = CAF_AIO_NOOP;
				if ((fstat (r->iocb.aio_fildes, &(r->sd))) == 0) {
					return r;
				} else {
					close (r->iocb.aio_fildes);
					xfree (r->path);
					xfree (r);
					r = (caf_aio_file_t *)NULL;
				}
			}
			r->buf = cbuf_create ((fs == CAF_OK ? (size_t)r->sd.st_blksize : bsz));
			if (r->buf != (cbuffer_t *)NULL) {
				cbuf_clean (r->buf);
				r->iocb.aio_buf = r->buf->data;
				r->iocb.aio_nbytes = (fs == CAF_OK ? (size_t)r->sd.st_blksize : r->buf->sz);
				r->iocb.aio_offset = 0;
				CAF_AIO_CLEAN(r);
			} else {
				close (r->iocb.aio_fildes);
				xfree (r->path);
				xfree (r);
				r = (caf_aio_file_t *)NULL;
			}
		} else {
			xfree (r);
			r = (caf_aio_file_t *)NULL;
		}
	}
	return r;
}


int
caf_aio_fclose (caf_aio_file_t *r) {
	if (r == (caf_aio_file_t *)NULL) {
		return CAF_ERROR;
	}
	if ((close (r->iocb.aio_fildes)) == 0) {
		cbuf_delete (r->buf);
		xfree (r->path);
		xfree (r);
		return CAF_OK;
	}
	return CAF_ERROR;
}


caf_aio_file_t *
caf_aio_reopen (caf_aio_file_t *r) {
	caf_aio_file_t *r2 = (caf_aio_file_t *)NULL;
	if (r == (caf_aio_file_t *)NULL) {
		return r;
	}
	r2 = caf_aio_fopen (r->path, r->flags, r->mode, r->ustat, r->buf->sz);
	if (r2 != (caf_aio_file_t *)NULL) {
		if ((caf_aio_fclose (r)) == CAF_OK) {
			return r2;
		} else {
			caf_aio_fclose (r2);
			r2 = (caf_aio_file_t *)NULL;
		}
	}
	return r2;
}


int
caf_aio_restat (caf_aio_file_t *r) {
	struct stat sb;
	if (r != (caf_aio_file_t *)NULL
		&& (fstat (r->fd, &sb)) == 0) {
		r->sd = sb;
		r->lastop = CAF_AIO_RSTAT;
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_aio_fchanged (caf_aio_file_t *r, struct timespec *lmt,
				  struct timespec *lct) {
	time_t diff;
	if (r != (caf_aio_file_t *)NULL) {
		diff = (r->sd.st_mtime - lmt->tv_sec) +
			(r->sd.st_ctime - lct->tv_sec);
		if (diff != 0) {
			return CAF_OK;
		} else {
			return CAF_ERROR;
		}
	}
	return CAF_ERROR_SUB;
}


int
caf_aio_read (caf_aio_file_t *r, cbuffer_t *b) {
	int rd = -1;
	int prev_errno;
	if (r != (caf_aio_file_t *)NULL
		&& b != (cbuffer_t *)NULL
		&& (r->fd >= 0 && (b->sz > 0 || b->iosz > 0))) {
		prev_errno = errno;
		errno = 0;
		rd = aio_read (&(r->iocb));
		r->aio_errno = errno;
		errno = prev_errno;
		r->lastop = CAF_AIO_READ;
	}
	return rd;
}


int
caf_aio_write (caf_aio_file_t *r, cbuffer_t *b) {
	int wr = -1;
	int prev_errno;
	if (r != (caf_aio_file_t *)NULL
		&& b != (cbuffer_t *)NULL
		&& ((b->iosz > 0 || b->sz > 0) && r->fd >= 0)) {
		prev_errno = errno;
		errno = 0;
		wr = aio_write (&(r->iocb));
		r->aio_errno = errno;
		errno = prev_errno;
		r->lastop = CAF_AIO_WRITE;
	}
	return wr;
}


int
caf_aio_fcntl (caf_aio_file_t *r, int cmd, int *arg) {
	int opr = CAF_ERROR_SUB;
	int prev_errno;
	if (r != (caf_aio_file_t *)NULL) {
		prev_errno = errno;
		errno = 0;
		if (arg != (int *)NULL) {
			opr = fcntl (r->fd, cmd, *arg);
		} else {
			opr = fcntl (r->fd, cmd);
		}
		r->aio_errno = errno;
		errno = prev_errno;
		r->lastop = CAF_AIO_FCNTL;
	}
	return opr;
}


int
caf_aio_flseek (caf_aio_file_t *r, off_t o, int w) {
	int prev_errno;
	if (r != (caf_aio_file_t *)NULL) {
		prev_errno = errno;
		errno = 0;
		if ((lseek (r->fd, o, w)) > -1) {
			r->lastop = CAF_AIO_LSEEK;
			return CAF_OK;
		}
		r->aio_errno = errno;
		errno = prev_errno;
	}
	return CAF_ERROR;
}


int
caf_aio_cancel (caf_aio_file_t *r) {
	int op = CAF_AIO_CANCELBADF;
	int prev_errno;
	if (r != (caf_aio_file_t *)NULL) {
		prev_errno = errno;
		errno = 0;
		op = aio_cancel (r->iocb.aio_fildes, &(r->iocb));
		r->aio_errno = errno;
		errno = prev_errno;
		switch (op) {
		case AIO_CANCELED:
			op = CAF_AIO_CANCELED;
			break;
		case AIO_NOTCANCELED:
			op = CAF_AIO_NOTCANCELED;
			break;
		case AIO_ALLDONE:
			op = CAF_AIO_ALLDONE;
			break;
		}
		r->lastop = CAF_AIO_CANCEL;
	}
	return op;
}


int
caf_aio_return (caf_aio_file_t *r) {
	int s = -1;
	int prev_errno;
	if (r != (caf_aio_file_t *)NULL) {
		prev_errno = 0;
		errno = 0;
		s = aio_return (&(r->iocb));
		r->aio_errno = errno;
		errno = prev_errno;
	}
	return s;
}


int
caf_aio_error (caf_aio_file_t *r) {
	int s = -1;
	int prev_errno;
	if (r != (caf_aio_file_t *)NULL) {
		prev_errno = errno;
		errno = 0;
		s = aio_error (&(r->iocb));
		r->aio_errno = errno;
		errno = prev_errno;
	}
	return s;
}


int
caf_aio_suspend (caf_aio_file_t *r, const struct timespec *to) {
	int s = -1;
	int prev_errno;
	struct aiocb *l_iocb[1];
	if (r != (caf_aio_file_t *)NULL
		&& to != (const struct timespec *)NULL) {
		l_iocb[0] = &(r->iocb);
		prev_errno = errno;
		errno = 0;
		s = aio_suspend ((const struct aiocb * const *)l_iocb, 1, to);
		r->aio_errno = errno;
		errno = prev_errno;
	}
	return s;
}


#ifdef HAVE_AIO_WAITCOMPLETE
int
caf_aio_waitcomplete (caf_aio_file_t *r, struct timespec *to) {
	int s = -1;
	int prev_errno;
	struct aiocb *l_iocb[1];
	if (r != (caf_aio_file_t *)NULL
		&& to != (const struct timespec *)NULL) {
		l_iocb[0] = &(r->iocb);
		prev_errno = errno;
		errno = 0;
		s = aio_waitcomplete ((struct aiocb **)l_iocb, to);
		r->aio_errno = errno;
		errno = prev_errno;
	}
	return s;
}
#endif /* !HAVE_AIO_WAITCOMPLETE */


caf_aio_file_lst_t *
caf_aio_lst_new (const int flg, const mode_t md, int fs, int count) {
	size_t tsz, fdsz;
	caf_aio_file_lst_t *r = (caf_aio_file_lst_t *)NULL;
	int c;
	if (count > 0) {
		tsz = CAF_AIO_CTRLB_SZ * (size_t)count;
		r = (caf_aio_file_lst_t *)xmalloc (tsz);
		if (r != (caf_aio_file_lst_t *)NULL) {
			memset (r, 0, tsz);
			r->flags = flg;
			r->ustat = fs;
			r->mode = md;
			fdsz = (size_t)count * sizeof(int);
			r->iocb_fds = (int *)xmalloc (fdsz);
			for (c = 0; c <= count; c++) {
				r->iocb_fds[c] = -1;
			}
		}
	}
	return r;
}


int
caf_aio_lst_delete (caf_aio_file_lst_t *r) {
	if (r != (caf_aio_file_lst_t *)NULL) {
		xfree (r);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_aio_lst_open (caf_aio_file_lst_t *r, const char **paths) {
	int ofc = 0, c;
	if (r != (caf_aio_file_lst_t *)NULL && paths != (const char **)NULL) {
		r->iocb_paths = (char **)paths;
		for (c = 0; c < r->iocb_count; c++) {
			if ((io_can_open(paths[c], r->flags)) == CAF_OK) {
				if (r->mode != 0) {
					r->iocb_fds[c] = open (paths[c], r->flags, r->mode);
				} else {
					r->iocb_fds[c] = open (paths[c], r->flags);
				}
				if (r->iocb_fds[c] >= 0) {
					ofc++;
				}
			}
		}
	}
	return ofc;
}


int
caf_aio_lst_close (caf_aio_file_lst_t *r) {
	int ofc = 0, c;
	if (r != (caf_aio_file_lst_t *)NULL) {
		for (c = 0; c < r->iocb_count; c++) {
			if (r->iocb_fds[c] > -1) {
				if ((close (r->iocb_fds[c])) == 0) {
					r->iocb_fds[c] = -1;
					ofc++;
				}
			}
		}
	}
	return ofc;
}


int
caf_aio_lst_suspend (caf_aio_file_lst_t *r, const struct timespec *to,
					 int idx, int cnt) {
	struct aiocb **p = (struct aiocb **)NULL;
	int s = -1;
	int prev_errno;
	if (r != (caf_aio_file_lst_t *)NULL
		&& to != (const struct timespec *)NULL
		&& idx >= 0) {
		if (idx < r->iocb_count
			&& (idx + cnt) <= r->iocb_count) {
			p = (struct aiocb **)&(r->iocb_list[idx]);
			prev_errno = errno;
			errno = 0;
			s = aio_suspend ((const struct aiocb * const *)p,
							 cnt, to);
			r->aio_errno = errno;
			errno = prev_errno;
		}
	}
	return s;
}


#ifdef HAVE_AIO_WAIT_COMPLETE
int
caf_aio_lst_waitcomplete (caf_aio_file_lst_t *r, struct timespec *to,
						  int idx) {
	struct aiocb **p = (struct aiocb **)NULL;
	int s = -1;
	int prev_errno;
	if (r != (caf_aio_file_lst_t *)NULL
		&& to != (const struct timespec *)NULL
		&& idx >= 0) {
		if (idx < r->iocb_count) {
			p = (struct aiocb **)&(r->iocb_list[idx]);
			prev_errno = errno;
			errno = 0;
			s = aio_waitcomplete (p, to);
			r->aio_errno = errno;
			errno = prev_errno;
		}
	}
	return s;
}
#endif /* !HAVE_AIO_WAIT_COMPLETE */


int
caf_aio_lst_operation (caf_aio_file_lst_t *r, struct sigevent *e,
					   int mode) {
	struct aiocb **p = (struct aiocb **)NULL;
	int s = -1;
	int prev_errno;
	if (r != (caf_aio_file_lst_t *)NULL
		&& e != (struct sigevent *)NULL) {
		p = (struct aiocb **)&(r->iocb_list);
		prev_errno = errno;
		errno = 0;
		s = lio_listio (mode, (struct aiocb * const *)p, r->iocb_count,
						e);
		r->aio_errno = errno;
		errno = prev_errno;
	}
	return -1;
}


/* caf_aio_core.c ends here */

