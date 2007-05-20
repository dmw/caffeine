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

    $Id$
 */
#ifndef CAF_IO_TAIL_H
#define CAF_IO_TAIL_H 1
/**
 * @defgroup      caf_io_tail           I/O Tails
 * @ingroup       caf_io
 * @addtogroup    caf_io_tail
 * @{
 *
 * @brief     I/O Tails
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * I/O Tails
 *
 */

#include <caf/caf_data_buffer.h>

#include <caf/caf_io_file.h>
#include <caf/caf_evt_fio.h>

#define CAF_TAIL_STREAM_SZ          (sizeof (caf_tail_stream_t))

typedef struct caf_tail_stream_s caf_tail_stream_t;
struct caf_tail_stream_s {
    int status;
    int complete;
    int reset;
    off_t offset;
    caf_io_file_t *file;
    fio_evt_t *events;
};

caf_tail_stream_t *caf_tail_open (const char *path, int to);
int caf_tail_close (caf_tail_stream_t *s);
int caf_tail_read (caf_tail_stream_t *stream, cbuffer_t *buffer);
off_t caf_tail_getoffset (caf_tail_stream_t *stream, cbuffer_t *buffer);

/** }@ */
#endif /* !CAF_IO_TAIL_H */
/* caf_io_tail.h ends here */
