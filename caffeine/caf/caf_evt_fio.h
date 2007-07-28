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
#ifndef CAF_EVT_FIO_H
#define CAF_EVT_FIO_H 1
/**
 * @defgroup      caf_event_fio        File I/O Events Manager Abstraction
 * @ingroup       caf_evt
 * @addtogroup    caf_event_fio
 * @{
 *
 * @brief     CAF Event Manager.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * File I/O Events Manager Abstraction
 *
 */

#ifdef BSD_SYSTEM
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#endif /* !IO_EVENT_USE_KEVENT */

#ifdef LINUX_SYSTEM
#include <sys/inotify.h>
#endif /* !IO_EVENT_USE_KEVENT */

#include <caf/caf_io_file.h>

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

	/**
	 *
	 * @brief    Defines which I/O Events can be handled
	 * Defines the types of I/O Events that Caffeine can handle -- only the most
	 * portable events can be handled by Caffeine.
	 */
	typedef enum {
	    /** Read Events */
	    EVT_IO_READ = 0000001,
	    /** Write Events */
	    EVT_IO_WRITE = 0000002,
	    /** VNode Events */
	    EVT_IO_VNODE = 0000004,
	} fio_event_types_t;

#if defined(BSD_SYSTEM) && !defined(IO_EVENT_DATA_KEVENT_SZ)
#define IO_EVENT_DATA_KEVENT_SZ     (sizeof (io_evt_kevent_t))
#endif /* !BSD_SYSTEM */
#if defined(LINUX_SYSTEM) && !defined(IO_EVENT_DATA_INOTIFY_SZ)
#define IO_EVENT_DATA_INOTIFY_SZ      (sizeof (io_evt_inotify_t))
#endif /* !LINUX_SYSTEM */

#if defined(BSD_SYSTEM) && !defined(IO_EVT_KEVENT_T_DEFINED)
#define IO_EVT_KEVENT_T_DEFINED 1
	typedef struct kevent io_evt_kevent_t;
#endif /* !BSD_SYSTEM */

#if defined(LINUX_SYSTEM) && !defined(IO_EVT_INOTIFY_T_DEFINED)
#define IO_EVT_INOTIFY_T_DEFINED 1
	typedef struct inotify_event io_evt_inotify_t;
#endif /* !LINUX_SYSTEM */


	typedef struct caf_evt_fio_event_mapping_s caf_evt_fio_event_mapping_t;
	struct caf_evt_fio_event_mapping_s {
		int id;
		int evt_kevent;
		int evt_inotify;
	};


	typedef enum {
	    IO_EVENTS_KEVENT,
	    IO_EVENTS_INOTIFY
	} fio_evt_use_t;


#define IO_EVT_SZ               (sizeof (fio_evt_t))
	typedef struct fio_evt_s fio_evt_t;
	struct fio_evt_s {
		int ev_src;
		caf_io_file_t *ev_mf;
		int ev_timeout;
		int ev_type;
		fio_evt_use_t ev_use;
		void *ev_info;
		void *ev_store;
		size_t ev_sz;
	};

#define EVT_IO_READ_IDX             0
#define EVT_IO_WRITE_IDX            1
#define EVT_IO_VNODE_IDX            2

	/* common */
	int fio_evt_events (fio_evt_t *e);
	int fio_evt_events_use (fio_evt_t *e, fio_evt_use_t type);

#if defined(BSD_SYSTEM) && !defined(IO_EVENT_USE_KEVENT)
#define IO_EVENT_USE_KEVENT 1
#elif defined(LINUX_SYSTEM) && !defined(IO_EVENT_USE_INOTIFY)
#define IO_EVENT_USE_INOTIFY 1
#endif /* !LINUX_SYSTEM */

#if defined(IO_EVENT_USE_KEVENT)
#define CALL_EVT_F(p)             p##_kevent
#elif defined(IO_EVENT_USE_INOTIFY)
#define CALL_EVT_F(p)             p##_inotify
#endif /* !IO_EVENT_USE_* */

	fio_evt_t *CALL_EVT_F(fio_evt_new) (caf_io_file_t *f, int type, int to);
	int CALL_EVT_F(fio_evt_delete) (fio_evt_t *e);
	int CALL_EVT_F(fio_evt_init) (fio_evt_t *e);
	int CALL_EVT_F(fio_evt_reinit) (fio_evt_t *e);
#ifndef LINUX_SYSTEM
	int CALL_EVT_F(fio_evt_add) (fio_evt_t *e, int ev, int flg);
#endif /* !LINUX_SYSTEM */
	int CALL_EVT_F(fio_evt_destroy) (fio_evt_t *e);
	int CALL_EVT_F(fio_evt_handle) (fio_evt_t *e);
	int CALL_EVT_F(fio_evt_isread) (fio_evt_t *e);
	int CALL_EVT_F(fio_evt_iswrite) (fio_evt_t *e);
	int CALL_EVT_F(fio_evt_isvnode) (fio_evt_t *e);


#define caf_fio_evt_new              CALL_EVT_F(fio_evt_new)
#define caf_fio_evt_delete           CALL_EVT_F(fio_evt_delete)
#define caf_fio_evt_init             CALL_EVT_F(fio_evt_init)
#define caf_fio_evt_reinit           CALL_EVT_F(fio_evt_reinit)
#define caf_fio_evt_add              CALL_EVT_F(fio_evt_add)
#define caf_fio_evt_destroy          CALL_EVT_F(fio_evt_destroy)
#define caf_fio_evt_handle           CALL_EVT_F(fio_evt_handle)
#define caf_fio_evt_isread           CALL_EVT_F(fio_evt_isread)
#define caf_fio_evt_iswrite          CALL_EVT_F(fio_evt_iswrite)
#define caf_fio_evt_isvnode          CALL_EVT_F(fio_evt_isvnode)

#ifdef __cplusplus
};
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_EVT_FIO_H */
/* caf_evt_fio.h ends here */
