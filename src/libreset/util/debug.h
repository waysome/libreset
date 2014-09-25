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

/**
 * @addtogroup internal-debug "(internal) Debugging helpers"
 *
 * @{
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__


/*
 * Fallback definition of __func__ for older gcc versions
 */
#if __STDC_VERSION__ < 199901L
#if __GNUC__ >= 2

#define __func__ __FUNCTION__

#else

#define __func__ "<unknown>"

#endif
#endif

#ifdef DEBUG
#include <stdio.h>

/**
 * debug macro which prints the current file, function and the line as well as
 * any given string
 */
#define dbg(fmt,...) do {                       \
        fprintf(stderr,                         \
                "[libreset][%s][%s][%s]: "fmt,  \
                __FILE__,                       \
                __func__,                       \
                __LINE__,                       \
                __VA_ARGS__);                   \
    } while (0)

#else

#define dbg(fmt,...)

#endif

#endif // __DEBUG_H__

/**
 * @}
 */
