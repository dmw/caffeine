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
#ifndef CAF_EVT_IO_H
#define CAF_EVT_IO_H 1
/**
 * @defgroup      caf_event_io        I/O Events Manager Abstraction
 * @ingroup       caf_evt
 * @addtogroup    caf_event_io
 * @{
 *
 * @brief     CAF Event Manager.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * I/O Events Manager Abstraction
 *
 */

#if defined(LINUX_SYSTEM)
#include <sys/poll.h>
#else
#include <poll.h>
#endif /* !LINUX_SYSTEM */

#include <sys/select.h>

#ifdef BSD_SYSTEM
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#endif /* !IO_EVENT_USE_KEVENT */

#ifdef LINUX_SYSTEM
#include <sys/epoll.h>
#endif /* !IO_EVENT_USE_KEVENT */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
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
	EVT_IO_WRITE = 0000002
} io_event_types_t;


#define IO_EVENT_DATA_SELECT_SZ     (sizeof (io_evt_select_t))
#define IO_EVENT_DATA_POLL_SZ       (sizeof (io_evt_poll_t))
#if defined(BSD_SYSTEM) && !defined(IO_EVENT_DATA_KEVENT_SZ)
#define IO_EVENT_DATA_KEVENT_SZ     (sizeof (io_evt_kevent_t))
#endif /* !BSD_SYSTEM */
#if defined(LINUX_SYSTEM) && !defined(IO_EVENT_DATA_EPOLL_SZ)
#define IO_EVENT_DATA_EPOLL_SZ      (sizeof (io_evt_epoll_t))
#endif /* !LINUX_SYSTEM */


typedef struct io_evt_poll_s io_evt_poll_t;
struct io_evt_poll_s {
	struct pollfd poll;
	struct timespec timeout;
};


typedef struct io_evt_select_s io_evt_select_t;
struct io_evt_select_s {
	int fd;
	fd_set rd;
	fd_set wr;
	struct timeval timeout;
};


#if defined(BSD_SYSTEM) && !defined(IO_EVT_KEVENT_T_DEFINED)
#define IO_EVT_KEVENT_T_DEFINED 1
typedef struct kevent io_evt_kevent_t;
#endif /* !BSD_SYSTEM */


#if defined(LINUX_SYSTEM) && !defined(IO_EVT_EPOLL_T_DEFINED)
#define IO_EVT_EPOLL_T_DEFINED 1
typedef struct epoll_event io_evt_epoll_t;
#endif /* !LINUX_SYSTEM */


typedef struct caf_evt_io_event_mapping_s caf_evt_io_event_mapping_t;
struct caf_evt_io_event_mapping_s {
	int id;
	int evt_poll;
	int evt_select;
	int evt_kevent;
	int evt_epoll;
};


typedef enum {
	IO_EVENTS_SELECT,
	IO_EVENTS_POLL,
	IO_EVENTS_KEVENT,
	IO_EVENTS_EPOLL
} io_evt_use_t;


#define IO_EVT_SZ               (sizeof (io_evt_t))

typedef struct io_evt_s io_evt_t;
struct io_evt_s {
	int ev_src;
	int ev_mfd;
	int ev_timeout;
	int ev_type;
	io_evt_use_t ev_use;
	void *ev_info;
	void *ev_store;
	size_t ev_sz;
};


#define EVT_IO_READ_IDX             0
#define EVT_IO_WRITE_IDX            1


/* common */
int io_evt_events (io_evt_t *e);
int io_evt_events_use (io_evt_t *e, io_evt_use_t type);


#if defined(IO_EVENT_USE_KEVENT)
#define CALL_EVT_F(p)             p##_kevent
#elif defined(IO_EVENT_USE_EPOLL)
#define CALL_EVT_F(p)             p##_epoll
#elif defined(IO_EVENT_USE_POLL)
#define CALL_EVT_F(p)             p##_poll
#elif defined(IO_EVENT_USE_SELECT)
#define CALL_EVT_F(p)             p##_select
#else
/* Fallback to poll(2) */
#define CALL_EVT_F(p)             p##_poll
#endif /* !IO_EVENT_USE_* */

io_evt_t *CALL_EVT_F(io_evt_new) (int fd, int type, int to);
int CALL_EVT_F(io_evt_delete) (io_evt_t *e);
int CALL_EVT_F(io_evt_init) (io_evt_t *e);
int CALL_EVT_F(io_evt_reinit) (io_evt_t *e);
#if defined(IO_EVENT_USE_KEVENT)
int CALL_EVT_F(io_evt_add) (io_evt_t *e, int ev, int flg);
#else
int CALL_EVT_F(io_evt_add) (io_evt_t *e, int ev);
#endif /* !IO_EVENT_USE_KEVENT */
int CALL_EVT_F(io_evt_destroy) (io_evt_t *e);
int CALL_EVT_F(io_evt_handle) (io_evt_t *e);
int CALL_EVT_F(io_evt_isread) (io_evt_t *e);
int CALL_EVT_F(io_evt_iswrite) (io_evt_t *e);


#define caf_io_evt_new              CALL_EVT_F(io_evt_new)
#define caf_io_evt_delete           CALL_EVT_F(io_evt_delete)
#define caf_io_evt_init             CALL_EVT_F(io_evt_init)
#define caf_io_evt_reinit           CALL_EVT_F(io_evt_reinit)
#define caf_io_evt_add              CALL_EVT_F(io_evt_add)
#define caf_io_evt_destroy          CALL_EVT_F(io_evt_destroy)
#define caf_io_evt_handle           CALL_EVT_F(io_evt_handle)
#define caf_io_evt_isread           CALL_EVT_F(io_evt_isread)
#define caf_io_evt_iswrite          CALL_EVT_F(io_evt_iswrite)

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_EVT_IO_H */
/* caf_evt_io.h ends here */
