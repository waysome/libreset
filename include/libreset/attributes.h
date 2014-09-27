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

#ifndef __LIBRESET_ATTRIBUTES_H__
#define __LIBRESET_ATTRIBUTES_H__

/*
 * @file attributes.h
 *
 * @brief Aliasing of compiler supported macros
 *
 * This file contains aliases for macros which are supported by the compiler.
 * For example, we alias __r_pure__ to __attribute__((pure)) here if we compile
 * with a gcc compiler.
 *
 * If the compiler does not support such macros, we define them to nothing, to
 * ensure we can still compile our code.
 *
 * Each alias is written in snake_case with two leading and two least
 * underscores, to produce a hint to the __attribute__ macro.
 *
 * We also define some aliases for ourselves in here, such as R_FORCE_INLINE,
 * which evaluates to 'inline __r_always_inline__'. As this is not _just_ an alias
 * for an compiler macro, we use the normal naming convention here.
 */

#ifdef __GNUC__

#define __r_always_inline__       __attribute__((always_inline))
#define __r_no_inline__           __attribute__((noinline))
#define __r_cold__                __attribute__((cold))
#define __r_hot__                 __attribute__((hot))
#define __r_pure__                __attribute__((pure))
#define __r_const__               __attribute__((const))
#define __r_weak__                __attribute__((weak))
#define __r_alias__(x)            __attribute__((alias(x)))
#define __r_deprecated__          __attribute__((deprecated))
#define __r_fastcall__            __attribute__((fastcall))
#define __r_format__(x,y,z)       __attribute__((format(x,y,z)))
#define __r_format_arg__(n)       __attribute__((format_arg(n)))
#define __r_malloc__              __attribute__((malloc))
#define __r_nonnull__(...)        __attribute__((nonnull (__VA_ARGS__)))
#define __r_returns_nonnull__     __attribute__((returns_nonnull))
#define __r_noreturn__            __attribute__((noreturn))
#define __r_unused__              __attribute__((unused))
#define __r_visibility__(x)       __attribute__((visibility(x)))

#define __r_vis_default__     __r_visibility__(default)
#define __r_vis_hidden__      __r_visibility__(hidden)
#define __r_vis_internal__    __r_visibility__(internal)
#define __r_vis_protected__   __r_visibility__(protected)

#define __r_warn_unused_result__  __attribute__((warn_unused_result))

#define R_FORCE_INLINE         inline __r_always_inline__

#else // __GNUC__

#define __r_always_inline__
#define __r_no_inline__
#define __r_cold__
#define __r_hot__
#define __r_pure__
#define __r_const__
#define __r_weak__
#define __r_alias__(x)
#define __r_deprecated__
#define __r_fastcall__
#define __r_format__(x,y,z)
#define __r_format_arg__(n)
#define __r_malloc__
#define __r_nonnull__(...)
#define __r_returns_nonnull__
#define __r_noreturn__
#define __r_unused__
#define __r_visibility__(x)

#define __r_default__
#define __r_hidden__
#define __r_internal__
#define __r_protected__

#define __r_warn_unused_result__

#define R_FORCE_INLINE

#endif // __GNUC__

#endif //__LIBRESET_ATTRIBUTES_H__

/**
 * @}
 */
