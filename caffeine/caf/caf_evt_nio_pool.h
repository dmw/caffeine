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
#ifndef CAF_EVT_NIO_POOL_H
#define CAF_EVT_NIO_POOL_H 1
/**
 * @defgroup      caf_event_io_pool        I/O Events Pool Manager Abstraction
 * @ingroup       caf_evt
 * @addtogroup    caf_event_io_pool
 * @{
 *
 * @brief     I/O Events Pool Manager Abstraction
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * I/O Events Pool Manager Abstraction
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
#endif /* !BSD_SYSTEM */

#ifdef LINUX_SYSTEM
#include <sys/epoll.h>
#endif /* !LINUX_SYSTEM */

#define IO_EVENT_DATA_POOL_SELECT_SZ     (sizeof (io_evt_pool_poll_t))
#define IO_EVENT_DATA_POOL_POLL_SZ       (sizeof (io_evt_pool_select_t))
#define IO_EVENT_DATA_POLLFDS_SZ         (sizeof (struct pollfd))
#ifdef BSD_SYSTEM
#define IO_EVENT_DATA_POOL_KEVENT_SZ     (sizeof (io_evt_pool_kevent_t))
#define IO_EVENT_DATA_KEVENTS_SZ         (sizeof (struct kevent))
#endif /* !BSD_SYSTEM */
#ifdef LINUX_SYSTEM
#define IO_EVENT_DATA_POOL_EPOLL_SZ      (sizeof (io_evt_pool_epoll_t))
#define IO_EVENT_DATA_EPOLLS_SZ          (sizeof (struct epoll_event))
#endif /* !LINUX_SYSTEM */

#include <caf/caf_evt_nio.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

typedef struct io_evt_pool_poll_s io_evt_pool_poll_t;
struct io_evt_pool_poll_s {
	int poll_count;
	size_t poll_sz;
	struct pollfd *poll;
	struct timespec timeout;
};


typedef struct io_evt_pool_select_s io_evt_pool_select_t;
struct io_evt_pool_select_s {
	int fd;
	fd_set rd;
	fd_set wr;
	struct timeval timeout;
};


#ifdef BSD_SYSTEM
typedef struct io_evt_pool_kevent_s io_evt_pool_kevent_t;
struct io_evt_pool_kevent_s {
	int kevent_count;
	int kfd;
	size_t kevent_sz;
	struct kevent *kevent_src;
	struct kevent *kevent_dst;
	struct timespec timeout;
};
#endif /* !BSD_SYSTEM */


#ifdef LINUX_SYSTEM
typedef struct io_evt_pool_epoll_s io_evt_pool_epoll_t;
struct io_evt_pool_epoll_s {
	int epoll_count;
	int efd;
	size_t epoll_sz;
	struct epoll_event *epoll;
	struct epoll_event *repoll;
	struct timespec timeout;
};
#endif /* !LINUX_SYSTEM */


typedef enum {
	EVT_POOL_SEED_CONNECTION,
	EVT_POOL_SEED_SERVICE
} caf_evt_pool_type_t;


#define IO_EVT_POOL_SZ               (sizeof (io_evt_pool_t))

typedef struct io_evt_pool_s io_evt_pool_t;
struct io_evt_pool_s {
	int evp_id;
	int evp_count;
	caf_evt_pool_type_t type;
	size_t evp_src_sz;
	void *evp_src;
	void *evp_seed;
};


#if defined(IO_EVENT_USE_KEVENT)
#define CALL_EVT_FP(p,s)             p##_kevent_##s
#define EVT_FP_T                     io_evt_pool_kevent_t
#elif defined(IO_EVENT_USE_EPOLL)
#define CALL_EVT_FP(p,s)             p##_epoll_##s
#define EVT_FP_T                     io_evt_pool_epoll_t
#elif defined(IO_EVENT_USE_POLL)
#define CALL_EVT_FP(p,s)             p##_poll_##s
#define EVT_FP_T                     io_evt_pool_poll_t
#elif defined(IO_EVENT_USE_SELECT)
#define CALL_EVT_FP(p,s)             p##_select_##s
#define EVT_FP_T                     io_evt_pool_select_t
#else
/* Fallback to poll(2) */
#define CALL_EVT_FP(p,s)             p##_poll_##s
#define EVT_FP_T                     io_evt_pool_poll_t
#endif /* !IO_EVENT_USE_* */

EVT_FP_T *CALL_EVT_FP(io_evt_pool,new) (int cnt, int tos, int ton);
int CALL_EVT_FP(io_evt_pool,delete) (EVT_FP_T *r);
int CALL_EVT_FP(io_evt_pool,reset) (EVT_FP_T *e);
int CALL_EVT_FP(io_evt_pool,add) (int fd, EVT_FP_T *e, int ef);
int CALL_EVT_FP(io_evt_pool,hasevent) (int fd, EVT_FP_T *e, int ef);
int CALL_EVT_FP(io_evt_pool,getevent) (int fd, EVT_FP_T *e);
int CALL_EVT_FP(io_evt_pool,etype) (int fd, EVT_FP_T *e);
int CALL_EVT_FP(io_evt_pool,handle) (EVT_FP_T *e);

#define caf_io_evt_pool_new              CALL_EVT_FP(io_evt_pool,new)
#define caf_io_evt_pool_delete           CALL_EVT_FP(io_evt_pool,delete)
#define caf_io_evt_pool_reset            CALL_EVT_FP(io_evt_pool,reset)
#define caf_io_evt_pool_add              CALL_EVT_FP(io_evt_pool,add)
#define caf_io_evt_pool_hasevent         CALL_EVT_FP(io_evt_pool,hasevent)
#define caf_io_evt_pool_getevent         CALL_EVT_FP(io_evt_pool,getevent)
#define caf_io_evt_pool_etype            CALL_EVT_FP(io_evt_pool,etype)
#define caf_io_evt_pool_handle           CALL_EVT_FP(io_evt_pool,handle)

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_EVT_NIO_POOL_H */
/* caf_evt_nio_pool.h ends here */
