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
#define MIN(x,y)                                    \
    ((__typeof__(x)) x > (__typeof__(x)) y ?    \
     (__typeof__(x)) y : (__typeof__(x)) x)

/**
 * Get the bitcount of a type
 *
 * @note Doing a `x << 3` is equivalent to `x * 8`, so the number of bytes in a
 * type << 3 is the number of bits in the type.
 */
#define BITCOUNT(type) (sizeof((type)) << 3)

/**
 * Mask the `n` least significant bits.
 *
 * Example:
 *
 *  LSB_MASK((char) 4) = 0b00001111
 *
 */
#define LSB_MASK(n) (~MSB_MASK(n))

/**
 * Mask the `n` most significant bits.
 *
 * Example:
 *
 *  MSB_MASK((char) 4) = 0b11110000
 *
 * Using `__typeof__` here to be sure we actually create the same type as passed
 */
#define MSB_MASK(n) ((~(__typeof__((n)))0) << (n))

/**
 * Compute the power of 2 to a constant value
 */
#define CONSTPOW_TWO(m) (2 << (m))

/** @} */

#endif //__MACROS_H__
