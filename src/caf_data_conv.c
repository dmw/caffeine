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

#include <stdlib.h>
#include <errno.h>

#include "caf/caf.h"
#include "caf/caf_data_mem.h"
#include "caf/caf_data_conv.h"


long *
str2longp (char *num, int base) {
	long *rlong;
	int p_errno;
	if (num != (char *)NULL) {
		rlong = xmalloc (sizeof(long));
		if (rlong != (long *)NULL) {
			p_errno = errno;
			errno = 0;
			*rlong = strtol(num, (char **)NULL, base);
			if (errno == 0) {
				errno = p_errno;
				return rlong;
			}
			xfree (rlong);
			return (long *)NULL;
		}
	}
	return (long *)NULL;
}


long
str2long (char *num, int base) {
	long rlong;
	int p_errno;
	if (num != (char *)NULL) {
		p_errno = errno;
		errno = 0;
		rlong = strtol(num, (char **)NULL, base);
		if (errno == 0) {
			errno = p_errno;
			return rlong;
		}
		return 0;
	}
	return 0;
}


float *
str2floatp (char *num) {
	float *rfloat;
	int p_errno;
	if (num != (char *)NULL) {
		rfloat = xmalloc (sizeof(float));
		if (rfloat != (float *)NULL) {
			p_errno = errno;
			errno = 0;
			*rfloat = strtof(num, (char **)NULL);
			if (errno == 0) {
				errno = p_errno;
				return rfloat;
			}
			xfree (rfloat);
			return (float *)NULL;
		}
	}
	return (float *)NULL;
}


float
str2float (char *num) {
	float rfloat;
	int p_errno;
	if (num != (char *)NULL) {
		p_errno = errno;
		errno = 0;
		rfloat = strtof(num, (char **)NULL);
		if (errno == 0) {
			errno = p_errno;
			return rfloat;
		}
		return 0.0;
	}
	return 0.0;
}


double *
str2doublep (char *num) {
	double *rdouble;
	int p_errno;
	if (num != (char *)NULL) {
		rdouble = xmalloc (sizeof(double));
		if (rdouble != (double *)NULL) {
			p_errno = errno;
			errno = 0;
			*rdouble = strtod(num, (char **)NULL);
			if (errno == 0) {
				errno = p_errno;
				return rdouble;
			}
		}
		xfree (rdouble);
		return (double *)NULL;
	}
	return (double *)NULL;
}


double
str2double (char *num) {
	double rdouble;
	int p_errno;
	if (num != (char *)NULL) {
		p_errno = errno;
		errno = 0;
		rdouble = strtod(num, (char **)NULL);
		if (errno == 0) {
			errno = p_errno;
			return rdouble;
		}
		return 0.0;
	}
	return 0.0;
}


long double *
str2ldoublep (char *num) {
	long double *rdouble;
	int p_errno;
	if (num != (char *)NULL) {
		rdouble = xmalloc (sizeof(double));
		if (rdouble != (long double *)NULL) {
			p_errno = errno;
			errno = 0;
			*rdouble = (long double)strtod(num, (char **)NULL);
			if (errno == 0) {
				errno = p_errno;
				return rdouble;
			}
		}
		xfree (rdouble);
		return (long double *)NULL;
	}
	return (long double *)NULL;
}


long double
str2ldouble (char *num) {
	long double rdouble;
	int p_errno;
	if (num != (char *)NULL) {
		p_errno = errno;
		errno = 0;
		rdouble = (long double)strtod(num, (char **)NULL);
		if (errno == 0) {
			errno = p_errno;
			return rdouble;
		}
		return 0.0;
	}
	return 0.0;
}

/* caf_data_conv.c ends here */

