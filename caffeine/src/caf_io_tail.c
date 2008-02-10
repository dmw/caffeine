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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_buffer.h"
#include "caf/caf_io_file.h"

#ifdef BSD_SYSTEM
#define IO_EVENT_USE_KEVENT 1
#endif /* !BSD_SYSTEM */

#ifdef LINUX_SYSTEM
#define IO_EVENT_USE_INOTIFY 1
#endif /* !LINUX_SYSTEM */

#include "caf/caf_evt_fio.h"
#include "caf/caf_io_tail.h"

#define CAF_TAIL_HANDLES             (EVT_IO_WRITE | EVT_IO_VNODE)

caf_tail_stream_t *
caf_tail_open (const char *path, int to) {
	caf_tail_stream_t *s = (caf_tail_stream_t *)NULL;
	if (path != (const char *)NULL) {
		if ((access (path, R_OK)) == 0) {
			s = (caf_tail_stream_t *)xmalloc (CAF_TAIL_STREAM_SZ);
			if (s != (caf_tail_stream_t *)NULL) {
				s->file = io_fopen (path, (O_RDONLY | O_NONBLOCK), 0, CAF_OK);
				if (s->file != (caf_io_file_t *)NULL) {
					s->events = caf_fio_evt_new (s->file, CAF_TAIL_HANDLES,
					                             to);
					if (s->events == (fio_evt_t *)NULL) {
						if ((io_fclose (s->file)) == CAF_OK) {
							xfree (s);
							s = (caf_tail_stream_t *)NULL;
						} else {
							s->status = CAF_ERROR;
						}
					}
					s->complete = CAF_ERROR;
					s->status = CAF_OK;
					s->offset = 0;
				} else {
					xfree (s);
					s = (caf_tail_stream_t *)NULL;
				}
			}
		}
	}
	return s;
}


int
caf_tail_close (caf_tail_stream_t *s) {
	if (s != (caf_tail_stream_t *)NULL) {
		if (s->events != (fio_evt_t *)NULL) {
			if ((caf_fio_evt_delete (s->events)) != CAF_OK) {
				s->status = CAF_ERROR;
				return CAF_ERROR;
			}
		}
		if (s->file != (caf_io_file_t *)NULL) {
			if ((io_fclose (s->file)) != CAF_OK) {
				s->status = CAF_ERROR;
				return CAF_ERROR;
			}
		}
		xfree (s);
		return CAF_OK;
	}
	return CAF_ERROR;
}


int
caf_tail_read (caf_tail_stream_t *s, cbuffer_t *b) {
	off_t offs;
	if (s != (caf_tail_stream_t *)NULL && b != (cbuffer_t *)NULL) {
		if (s->complete == CAF_OK) {
			if ((caf_fio_evt_handle (s->events)) == CAF_OK) {
				if ((caf_fio_evt_iswrite (s->events)) == CAF_OK) {
					offs = caf_tail_getoffset(s, b);
					if ((io_restat (s->file)) == CAF_OK) {
						if ((io_flseek (s->file, offs, SEEK_SET)) == CAF_OK) {
							cbuf_clean (b);
							b->iosz = io_read (s->file, b);
							s->offset = offs;
							s->complete = (offs < (off_t)s->file->sd.st_size)
								? CAF_ERROR : CAF_OK;
							s->status = CAF_OK;
							return CAF_OK;
						}
					} else {
						s->reset = CAF_OK;
						s->status = CAF_ERROR;
						return CAF_ERROR;
					}
				}
			}
		} else {
			if ((io_restat (s->file)) == CAF_OK) {
				offs = caf_tail_getoffset(s, b);
				if ((io_flseek (s->file, offs, SEEK_SET)) == CAF_OK) {
					cbuf_clean (b);
					b->iosz = io_read (s->file, b);
					s->offset = offs;
					s->complete = (offs != (off_t)s->file->sd.st_size)
						? CAF_ERROR : CAF_OK;
					s->status = CAF_OK;
					return CAF_OK;
				}
			} else {
				s->reset = CAF_OK;
				s->status = CAF_ERROR;
				return CAF_ERROR;
			}
		}
	}
	return CAF_ERROR;
}


off_t
caf_tail_getoffset (caf_tail_stream_t *stream, cbuffer_t *buffer) {
	off_t r = 0;
	if (stream != (caf_tail_stream_t *)NULL && buffer != (cbuffer_t *)NULL) {
		r = stream->offset + (off_t)buffer->iosz;
		if (r > (off_t)stream->file->sd.st_size) {
			r = (off_t)stream->file->sd.st_size;
		}
	}
	return r;
}

/* caf_io_tail.c ends here */

