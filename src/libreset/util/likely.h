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
 * @addtogroup internal-utils "(internal) Utilities"
 *
 * @{
 */

#ifndef __LIKELY_H__
#define __LIKELY_H__

/**
 * @file likely.h
 *
 * This file contains the likely() and unlikely() macro definitions
 */

#ifdef __GNUC__

#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

#else

#define likely(x)   (x)
#define unlikely(x) (x)

#endif //__GNUC__

#endif

/**
 * @}
 */
