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
#ifndef CAF_DATA_CONV_H
#define CAF_DATA_CONV_H 1
/**
 * @defgroup      caf_data_conversion    Data Conversion
 * @ingroup       caf_data_string
 * @addtogroup    caf_data_conversion
 * @{
 *
 * @brief     Caffeine Data Conversion.
 * @date      $Date$
 * @version   $Revision$
 * @author    Daniel Molina Wegener <dmw@unete.cl>
 *
 * Data conversion functions and defines. Here you can search for
 * functions which converts from one type to another. Usually made
 * by standard and well known functions and type casts.
 *
 */

#ifdef __cplusplus
CAF_BEGIN_C_EXTERNS
#endif /* !__cplusplus */

/**
 *
 * @brief    Converts a string to long pointer.
 *
 * Converts a string to native long type, also receibes a parameter
 * with the base that could be 8, 10 or 16. If the conversion have
 * not success, returns a NULL pointer.
 *
 * @param[in]    num             number string to convert.
 * @param[in]    base            base for the number.
 * @return       long *          a new allocated long pointer.
 */
long *str2longp(char *num, int base);

/**
 *
 * @brief    Converts a string to long.
 *
 * Converts a string to native long type, also receibes a parameter
 * with the base that could be 8, 10 or 16. If the conversion have
 * not success, returns a zero long value.
 *
 * @param[in]    num             number string to convert.
 * @param[in]    base            base for the number.
 * @return       long *          a new allocated long pointer.
 */
long str2long(char *num, int base);

/**
 *
 * @brief    Converts a string to float pointer.
 *
 * Converts a string to native float type, If the conversion have
 * not success, returns a NULL pointer.
 *
 * @param[in]    num             number string to convert.
 * @return       float *         a new allocated float pointer.
 */
float *str2floatp(char *num);

/**
 *
 * @brief    Converts a string to float.
 *
 * Converts a string to native float type, If the conversion have
 * not success, returns a zero float value.
 *
 * @param[in]    num             number string to convert.
 * @return       float *         a new allocated float pointer.
 */
float str2float(char *num);

/**
 *
 * @brief    Converts a string to double pointer.
 *
 * Converts a string to native double type, If the conversion have
 * not success, returns a NULL pointer.
 *
 * @param[in]    num             number string to convert.
 * @return       double *        a new allocated double pointer.
 */
double *str2doublep(char *num);

/**
 *
 * @brief    Converts a string to double.
 *
 * Converts a string to native double type, If the conversion have
 * not success, returns a zero double value.
 *
 * @param[in]    num             number string to convert.
 * @return       double *        a new allocated double pointer.
 */
double str2double(char *num);

/**
 *
 * @brief    Converts a string to long double pointer.
 *
 * Converts a string to native long double type, If the conversion have
 * not success, returns a NULL pointer.
 *
 * @param[in]    num             number string to convert.
 * @return       long double *   a new allocated long double pointer.
 */
long double *str2ldoublep(char *num);

/**
 *
 * @brief    Converts a string to long double.
 *
 * Converts a string to native long double type, If the conversion have
 * not success, returns zero long double value.
 *
 * @param[in]    num             number string to convert.
 * @return       long double *   a new allocated long double pointer.
 */
long double str2ldouble(char *num);

#ifdef __cplusplus
CAF_END_C_EXTERNS
#endif /* !__cplusplus */

/** }@ */
#endif /* !CAF_DATA_CONV_H */
/* caf_data_conv.h ends here */
