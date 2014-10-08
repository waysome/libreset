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

#include "libreset/attributes.h"
#include "libreset/hash.h"

/**
 * The set type
 */
struct r_set;


/**
 * Set configuration type
 */
struct r_set_cfg {
    r_hash         (*hashf)(void const* data); //!< hash function
    int             (*cmpf)(void const*, void const*); //!< compare function
    void*           (*copyf)(void*); //!< copy function
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
 * Insertion/processing function
 *
 * Use functions of this type if you want to select or process elements of a
 * set.
 * The function is expected to return 0 on success and negative values on error.
 * The first parameter is considered a user defined value (e.g. another set to
 * insert those values), while the elements will be fed to the function as the
 * second parameter.
 */
typedef int (*r_procf)(void*, void const*);



/**
 * Allocate and initialize set object
 *
 * @memberof r_set
 *
 * @return A pointer to the set object or NULL on failure
 */
struct r_set*
r_set_new(
    struct r_set_cfg const* cfg //!< configuration for the set object
)
__r_nonnull__(1)
;


/**
 * Remove a set object from memory
 *
 * @memberof r_set
 *
 * @return 0 on success, else errno const:
 *         -EEXIST - if the set doesn't exist (NULL passed)
 */
int
r_set_destroy(
    struct r_set* set //!< Set to remove
)
__r_nonnull__(1)
;


/**
 * Insert an object into a set
 *
 * @return zero on success, else error code (errno.h):
 *         -ENOMEM - on allocation failed
 *         -EEXIST - if the element is already in the set
 */
int
r_set_insert(
    struct r_set* set, //!< the set
    void* value //!< pointer to the value to insert
)
__r_nonnull__(1)
;

/**
 * Remove an object from the set
 *
 * @memberof r_set
 *
 * @return 0 (zero) on success and errno const:
 *         -EEXIST - if the element was not found in the set
 */
int
r_set_remove(
    struct r_set* set, //!< the set
    void const* cmp //!< object equal to the one you want to delete
)
__r_nonnull__(1, 2)
;


/**
 * Check if a set contains an object
 *
 * @memberof r_set
 *
 * @return The object if it is in the set, else NULL
 */
void*
r_set_contains(
    struct r_set const* set, //!< the set
    void const* cmp //!< element to check for
)
__r_nonnull__(1, 2)
;


/**
 * Compute union out of two sets
 *
 * @memberof r_set
 *
 * @return zero on success, else error code:
 *         -ENOMEM - if allocation failed
 */
int
r_set_union(
    struct r_set* dest, //!< destination of the result
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
)
__r_nonnull__(1, 2, 3)
;

/**
 * Compute intersection of two sets
 *
 * @memberof r_set
 *
 * @return zero on success, else error code:
 *         -ENOMEM - if allocation failed
 */
int
r_set_intersection(
    struct r_set* dest, //!< destination of the result
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
)
__r_nonnull__(1, 2, 3)
;

/**
 * Compute set with elements which are in only one of the two arguments
 *
 * @memberof r_set
 *
 * @return zero on success, else error code:
 *         -ENOMEM - if allocation failed
 */
int
r_set_xor(
    struct r_set* dest, //!< destination of the result
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
)
__r_nonnull__(1, 2, 3)
;

/**
 * Exclude elements from set_a which are in set_b
 *
 * @memberof r_set
 *
 * @return zero on success, else error code:
 *         -ENOMEM - if allocation failed
 */
int
r_set_exclude(
    struct r_set* dest, //!< destination of the result
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
)
__r_nonnull__(1, 2, 3)
;

/**
 * Check if one set is a subset of another
 *
 * @memberof r_set
 *
 * @return 1 if the first set is a subset of the second one, else zero (0)
 */
int
r_set_is_subset(
    struct r_set const* set_a, //!< first argument of the binary operation
    struct r_set const* set_b //!< second argument of the binary operation
)
__r_nonnull__(1, 2)
;

/**
 * Check if two sets are equal
 *
 * @memberof r_set
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
)
__r_nonnull__(1, 2)
;

/**
 * Get the cardinality of a set
 *
 * @memberof r_set
 *
 * @return the cardinality of the set
 */
size_t
r_set_cardinality(
    struct r_set const* set //!< the set to get the cardinality for
)
__r_nonnull__(1)
;

/**
 * Process selected entries from a set
 *
 * This function iterates over all the elements in the set.
 * First, a predicate (given by pred) is applied.
 * The first parameter of pred will be the element, while pred_etc will be
 * passed as the second parameter.
 * If the predicate function returns 1, the element is selected.
 * procf will be called, with the value given by dest as the first parameter
 * and the element to process as the second one.
 * If the function returns a negative result, no other elements will be
 * processed.
 * Instead, the negative (return) value will be passed to the user as the
 * return value of r_set_select itself.
 *
 * If NULL is passed for the predicate, all the elements in the set will be
 * selected.
 *
 * @memberof r_set
 *
 * @warning the function will crash if procf is NULL
 *
 * @return 0 or the return value of the last call to procf, if it returned a
 *         negative value
 */
int
r_set_select(
    struct r_set* src, //!< set with elements to process
    r_predf pred, //!< The predicate
    void* pred_etc, //!< Additional information for the predicate function
    r_procf procf, //!< function processing the selected values
    void* dest //!< some pointer to pass to the procf
);

#endif //__LIBRESET_H__
