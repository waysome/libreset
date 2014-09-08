/*
 * libreset - Reentrent set library for fast set operations in C
 *
 * Copyright (C) 2014 Matthias Beyer
 * Copyright (C) 2014 Julian Ganz
 *
 * This file is part of libreset.
 *
 * libreset is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * libreset is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libreset. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __MACROS_H__
#define __MACROS_H__

/**
 * @file macros.h
 *
 * This file contains simple helper macros
 */

/**
 * @addtogroup internal_util_helper_macros "(internal) helper macros"
 *
 * This group contains helper macros for internal use only.
 *
 * @{
 */

/**
 * Computes the maximum value of the two passed values
 *
 * @note Provides compile-time type checking by using temp variables before
 * doing the comparison.
 *
 * @note Opens own scope, so the temp variables do not show up outside of the
 * macro.
 */
#define MAX(x,y)                    \
       ({ __typeof__ (x) _x = (x);  \
          __typeof__ (y) _y = (y);  \
          _x > _y ? _x : _y; })

/**
 * Computes the minimum value of the two passed values
 *
 * @note Provides compile-time type checking by using temp variables before
 * doing the comparison.
 *
 * @note Opens own scope, so the temp variables do not show up outside of the
 * macro.
 */
#define MIN(x,y)                    \
       ({ __typeof__ (x) _x = (x);  \
          __typeof__ (y) _y = (y);  \
          _x < _y ? _x : _y; })

/** @} */

#endif //__MACROS_H__
