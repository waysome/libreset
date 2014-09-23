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
#ifndef __LIBRESET_H__
#define __LIBRESET_H__

#include <stddef.h>

#include "libreset/hash.h"

/**
 * The set type
 */
struct r_set;


/**
 * Set configuration type
 */
struct r_set_cfg {
    rs_hash         (*hashf)(void const* data); //!< hash function
    int             (*cmpf)(void const*, void const*); //!< compare function
    void const*     (*copyf)(void const*); //!< copy function
    void            (*freef)(void*); //!< function for removal/freeing of an item
};


/**
 * Predicate function type
 *
 * Use this function to express a predicate. The first parameter will be the
 * item to select from the set for an operation, the second parameter may be
 * used for any purpose. A predicate function selects an item by returning 1
 * and deselects an item by returning 0.
 */
typedef int (*r_predf)(void const*, void*);


/**
 * Allocate and initialize set object
 *
 * @return A pointer to the set object or NULL on failure
 */
struct r_set*
r_set_new(
    struct r_set_cfg const* cfg //!< configuration for the set object
);


/**
 * Remove a set object from memory
 */
void
r_set_destroy(
    struct r_set* set //!< Set to remove
);


/**
 * Insert an object into a set
 *
 * @return zero on success, else error code
 */
int
r_set_insert(
    struct r_set* set, //!< the set
    void* value //!< pointer to the value to insert
);

/**
 * Remove an object from the set
 *
 * @return The removed object or NULL on failure/not found
 */
void*
r_set_remove(
    struct r_set* set, //!< the set
    void const* cmp //!< object equal to the one you want to delete
);


/**
 * Check if a set contains an object
 *
 * @return The object if it is in the set, else NULL
 */
void*
r_set_contains(
    struct r_set const* set, //!< the set
    void const* cmp //!< element to check for
);


/**
 * Compute union out of two sets
 *
 * @return zero on success, else error code
 */
int
r_set_union(
    struct r_set* dest, //!< destination of the result
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
);


/**
 * Compute intersection of two sets
 *
 * @return zero on success, else error code
 */
int
r_set_intersection(
    struct r_set* dest, //!< destination of the result
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
);


/**
 * Compute set with elements which are in only one of the two arguments
 *
 * @return zero on success, else error code
 */
int
r_set_xor(
    struct r_set* dest, //!< destination of the result
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
);


/**
 * Exclude elements from set_a which are in set_b
 *
 * @return zero on success, else error code
 */
int
r_set_exclude(
    struct r_set* dest, //!< destination of the result
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
);


/**
 * Check if one set is a subset of another
 *
 * @return 1 if the first set is a subset of the second one, else zero (0)
 */
int
r_set_is_subset(
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
);


/**
 * Check if two sets are equal
 *
 * @note Checks metainformation of the set objects first (comparator function,
 * hashing function). If they are not equal, the function returns false (zero).
 *
 * @warning returns error codes on failure, which are true, too!
 *
 * @return 1 if the sets are equal, else 0 (zero)
 */
int
r_set_equal(
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
);


/**
 * Get the cardinality of a set
 *
 * @return the cardinality of the set
 */
size_t
r_set_cardinality(
    struct r_set const* set //!< the set to get the cardinality for
);


/**
 * Select entries from a set for a new set
 *
 * The predicate function gets two values passed, the first one is the actual
 * value for the predicate function to check, the second one is the parameter
 * one can pass through the r_set_select() function.
 *
 * @return zero on success, else error code
 */
int
r_set_select(
    struct r_set* dest, //!< destination set
    struct r_set const* src, //!< source set
    r_predf pred, //!< predicate for selection
    void* //!< parameter for the predicate function
);

#endif //__LIBRESET_H__
