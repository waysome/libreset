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

#ifndef __UTIL_ATTRIBUTES_H__
#define __UTIL_ATTRIBUTES_H__

/*
 * @file attributes.h
 *
 * @brief Aliasing of compiler supported macros
 *
 * This file contains aliases for macros which are supported by the compiler.
 * For example, we alias __pure__ to __attribute__((pure)) here if we compile
 * with a gcc compiler.
 *
 * If the compiler does not support such macros, we define them to nothing, to
 * ensure we can still compile our code.
 *
 * Each alias is written in snake_case with two leading and two least
 * underscores, to produce a hint to the __attribute__ macro.
 *
 * We also define some aliases for ourselves in here, such as RS_FORCE_INLINE,
 * which evaluates to 'inline __always_inline__'. As this is not _just_ an alias
 * for an compiler macro, we use the normal naming convention here.
 */

#ifdef __GNUC__

#define __always_inline__       __attribute__((always_inline))
#define __no_inline__           __attribute__((noinline))
#define __cold__                __attribute__((cold))
#define __hot__                 __attribute__((hot))
#define __pure__                __attribute__((pure))
#define __const__               __attribute__((const))
#define __weak__                __attribute__((weak))
#define __alias__(x)            __attribute__((alias(x)))
#define __deprecated__          __attribute__((deprecated))
#define __fastcall__            __attribute__((fastcall))
#define __format__(x,y,z)       __attribute__((format(x,y,z)))
#define __format_arg__(n)       __attribute__((format_arg(n)))
#define __malloc__              __attribute__((malloc))
#define __nonnull__(...)        __attribute__((nonnull ((__VA_ARGS__))))
#define __returns_nonnull__     __attribute__((returns_nonnull))
#define __noreturn__            __attribute__((noreturn))
#define __unused__              __attribute__((unused))
#define __visibility__(x)       __attribute__((visibility(x)))

#define __vis_default__     __visibility__(default)
#define __vis_hidden__      __visibility__(hidden)
#define __vis_internal__    __visibility__(internal)
#define __vis_protected__   __visibility__(protected)

#define __warn_unused_result__  __attribute__((warn_unused_result))

#define RS_FORCE_INLINE         inline __always_inline__

#else // __GNUC__

#define __always_inline__
#define __no_inline__
#define __cold__
#define __hot__
#define __pure__
#define __const__
#define __weak__
#define __alias__(x)
#define __deprecated__
#define __fastcall__
#define __format__(x,y,z)
#define __format_arg__(n)
#define __malloc__
#define __nonnull__(...)
#define __returns_nonnull__
#define __noreturn__
#define __unused__
#define __visibility__(x)

#define __default__
#define __hidden__
#define __internal__
#define __protected__

#define __warn_unused_result__

#define RS_FORCE_INLINE

#endif // __GNUC__

#endif //__UTIL_ATTRIBUTES_H__

/**
 * @}
 */
