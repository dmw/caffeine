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
#ifndef CAF_H
#define CAF_H 1
/**
 * @defgroup      caf        Caffeine - C Application Framework
 * @addtogroup    caf
 * @{
 *
 * @brief     Caffeine - C Application Framework
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Caffeine - C Application Framework
 */

#include <caf/caf_tool_macro.h>

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/**
 * @brief	Common Return Type under Caffeine
 *
 * This is the common return type under caffine, has two types of
 * errors, some functions return a number under -1 o upper to 0
 * both cases means an error. Only zero means success on common
 * operations.
 */
typedef enum {
	/** Error values under 0 */
	CAF_ERROR_SUB = -1,
	/** Success values are zero */
	CAF_OK = 0,
	/** Error values over 0 */
	CAF_ERROR = 1
} caf_return_t;

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_H */
/* caf.h ends here */
