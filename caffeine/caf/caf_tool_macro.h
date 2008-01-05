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

#ifndef CAF_TOOL_MACRO_H
#define CAF_TOOL_MACRO_H 1

#define CAF_MAX(x,y)         (x > y ? x : y)
#define CAF_FMAX(t,x,y)      ((t)x > (t)y ? (t)x : (t)y)

#define CAF_DEBUG(s)         \
    fprintf (stderr, "%s:%d: %s\n", __FILE__, __LINE__, s);

#define CAF_NULL_T(t)        ((t)NULL)
#define CAF_ISNULL(t,x)      (CAF_NULL_T(t) == x)

#ifndef CAF_BEGIN_C_EXTERNS
#define CAF_BEGIN_C_EXTERNS				extern "C" {
#endif /* !CAF_BEGIN_C_EXTERNS */

#ifndef CAF_END_C_EXTERNS
#define CAF_END_C_EXTERNS				};
#endif /* !CAF_END_C_EXTERNS */

#endif /* !CAF_TOOL_MACRO_H */
/* caf_tool_macro.h ends here */

