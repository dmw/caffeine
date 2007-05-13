/* -*- mode: c; c-default-style: "k&r"; indent-tabs-mode: nil; -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 expandtab: */
/* kate: space-indent on; indent-width 4; mixedindent off; indent-mode none; */
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


caf_io_file_t *
io_fopen (const char *path, const int flg, const mode_t md, int fs)
{
    caf_io_file_t *r = (caf_io_file_t *)NULL;
    if (path != (char *)NULL && flg > 0) {
        r = (caf_io_file_t *)xmalloc (CAF_IO_FILE_SZ);
        if (r != (caf_io_file_t *)NULL) {
            if (md != 0) {
                r->fd = open (path, flg, md);
            } else {
                r->fd = open (path, flg);
            }
            if (r->ustat >= 0) {
                r->flags = flg;
                r->mode = md;
                r->path = strdup (path);
                r->ustat = fs;
                if (fs == CAF_OK) {
                    if ((fstat (r->fd, &(r->sd))) == 0) {
                        return r;
                    } else {
                        close (r->fd);
                        xfree (r->path);
                        xfree (r);
                        r = (caf_io_file_t *)NULL;
                    }
                }
            }
        }
    }
    return r;
}


int
io_fclose (caf_io_file_t *r)
{
    if (r != (caf_io_file_t *)NULL) {
        if ((close (r->fd)) == 0) {
            xfree (r->path);
            xfree (r);
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


caf_io_file_t *
io_reopen (caf_io_file_t *r)
{
    caf_io_file_t *r2 = (caf_io_file_t *)NULL;
    if (r != (caf_io_file_t *)NULL) {
        r2 = io_fopen (r->path, r->flags, r->mode, r->ustat);
        if (r2 != (caf_io_file_t *)NULL) {
            if ((io_fclose (r)) == CAF_OK) {
                return r2;
            } else {
                io_fclose (r2);
                r2 = (caf_io_file_t *)NULL;
            }
        }
    }
    return r2;
}


int
io_restat (caf_io_file_t *r)
{
    struct stat sb;
    if (r != (caf_io_file_t *)NULL) {
        if ((fstat (r->fd, &sb)) == 0) {
            r->sd = sb;
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


int
io_fchanged (caf_io_file_t *r, struct timespec *lmt, struct timespec *lct)
{
    time_t diff;
    if (r != (caf_io_file_t *)NULL) {
        diff = (r->sd.st_mtime - lmt->tv_sec) + (r->sd.st_ctime - lct->tv_sec);
        return (int)diff;
    }
    return CAF_OK;
}


ssize_t
io_read (caf_io_file_t *r, cbuffer_t *b)
{
    ssize_t sz = 0;
    if (r != (caf_io_file_t *)NULL && b != (cbuffer_t *)NULL) {
        sz = read (r->fd, b->data, b->sz);
        b->iosz = sz;
    }
    return sz;
}


ssize_t
io_write (caf_io_file_t *r, cbuffer_t *b)
{
    ssize_t sz = 0;
    if (r != (caf_io_file_t *)NULL && b != (cbuffer_t *)NULL && sz > 0) {
        sz = write (r->fd, b->data, b->iosz);
    }
    return sz;
}


int
io_pipe (caf_io_file_t *r)
{
    if (r != (caf_io_file_t *)NULL) {
        return pipe (&(r->fd));
    }
    return CAF_ERROR_SUB;
}


int
io_fcntl (caf_io_file_t *r, int cmd, int *arg)
{
    if (r != (caf_io_file_t *)NULL) {
        if (arg != (int *)NULL) {
            return fcntl (r->fd, cmd, *arg);
        } else {
            return fcntl (r->fd, cmd);
        }
    }
    return CAF_ERROR_SUB;
}


int
io_flseek (caf_io_file_t *r, off_t o, int w)
{
    if (r != (caf_io_file_t *)NULL) {
        if ((lseek (r->fd, o, w)) == o) {
            return CAF_OK;
        }
    }
    return CAF_ERROR;
}


/* caf_io_core.c ends here */
