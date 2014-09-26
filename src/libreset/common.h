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

#ifndef __COMMON_H__
#define __COMMON_H__

#include <string.h>

#include "libreset/set.h"
#include "util/debug.h"

/**
 * Debug print helper for set implementation code
 *
 * @note No #ifdef DEBUG here, because if dbg() evaluates to nothing, this code
 * gets removed by the compiler anyways.
 */
#define set_dbg(fmt,...) do { dbg("set: "fmt, __VA_ARGS__); } while (0)

/**
 * Helper for comparing two `struct r_set_cfg` objects
 *
 * @return 1 if the passed config objects are equal, else zero
 */
static inline int
config_cmp(
    struct r_set_cfg const* a,
    struct r_set_cfg const* b
) {
    return ((a == b) || (a && b && (0 == memcmp(a, b, sizeof(*a)))));
}


#endif // __COMMON_H__
