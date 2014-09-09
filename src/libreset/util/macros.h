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
#define MAX(x,y)                                    \
        ((__typeof__(x)) x > (__typeof__(x)) y ?    \
         (__typeof__(x)) x : (__typeof__(x)) y)

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
