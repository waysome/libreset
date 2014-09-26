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
 * @addtogroup internal_ht_interface "(internal) Hashtable interface definition"
 *
 * This group contains the interface definition for the hashtable type used in
 * the codebase.
 *
 * @{
 */

#ifndef __HT_H__
#define __HT_H__

#include "libreset/hash.h"
#include "libreset/set.h"

#include "avl/avl.h"
#include "util/macros.h"

/**
 * Type for hashtable bucket
 *
 */
struct ht_bucket {
    struct avl avl;
};

/**
 * Hashtable type
 *
 * The type for the hashtable, holding buckets and size.
 */
struct ht {
    struct ht_bucket* buckets; //!< The buckets of the hashtable
    size_t sizeexp; //!< Exp., 2 must be raised to, to get the size of the ht
};

/**
 * Initialize a struct ht object
 *
 * @memberof ht
 *
 * @return the pointer to the passed struct ht object, NULL on failure
 */
struct ht*
ht_init(
    struct ht* ht, //!< The hashtable object to initialize
    size_t n //!< Power, 2 must be raised to, to calc the number of buckets.
)
__r_warn_unused_result__
;

/**
 * Destroy a struct ht object but don't free it
 *
 * @memberof ht
 *
 * @return 0 on success, else errno const:
 *         -EEXIST - if the ht doesn't exist (NULL passed, no buckets)
 */
int
ht_destroy(
    struct ht* ht, //!< The hashtable object to destory
    struct r_set_cfg const* cfg //!< type information provided by user
)
__r_nonnull__(1, 2)
;

/**
 * Find an element inside the hashtable by its hash and the predicate provided
 * by `cfg`
 *
 * @memberof ht
 *
 * @return the found element or NULL on failure
 */
void*
ht_find(
    struct ht const* ht, //!< The hashtable object to search in
    void const* cmp, //!< Element to compare against
    struct r_set_cfg const* cfg //!< type information provided by user
)
__r_nonnull__(1, 2, 3)
__r_warn_unused_result__
;

/**
 * Insert data into the hashtable
 *
 * @memberof ht
 *
 * @return 0 on success or negative error number on failure (errno.h)
 *         -ENOMEM - on allocation failed
 *         -EEXIST - if the element is already in the set
 */
int
ht_insert(
    struct ht* ht, //!< The hashtable object to insert into
    void* data, //!< The data
    struct r_set_cfg const* cfg //!< type information provided by user
)
__r_nonnull__(1, 2, 3)
;

/**
 * Delete one element from the hashtable by hash and the predicate provided by
 * `cfg`
 *
 * @memberof ht
 *
 * @return 0 if the insertion was successfull, else errno const:
 *         -EEXIST - if the element was not found in the ht
 */
int
ht_del(
    struct ht* ht, //!< The hashtable object to delete from
    void const* cmp, //!< Element to compare against
    struct r_set_cfg const* cfg //!< type information provided by user
)
__r_nonnull__(1, 2, 3)
;

/**
 * Get the cardinality of the hashtable (amount of elements)
 *
 * @memberof ht
 *
 * @return the cardinality of the hashtable
 */
size_t
ht_cardinality(
    struct ht const* ht //!< The hashtable object to delete from
)
__r_nonnull__(1)
__r_warn_unused_result__
;

/**
 * Deleting n elements from the hashtable by predicate
 *
 * @memberof ht
 *
 * @return the number of removed elements.
 */
unsigned int
ht_ndel(
    struct ht* ht, //!< The hashtable object to delete from
    r_predf pred, //!< predicate function
    void* etc, //!< element to compare against
    struct r_set_cfg const* cfg //!< type information provided by the user
)
__r_nonnull__(1, 3, 4)
;

/**
 * Helper to calculate the actual bucket count of the hashtable
 *
 * @memberof ht
 *
 * @return The number of buckets in `ht` or zero on failure.
 */
static inline size_t
ht_nbuckets(
    struct ht const* const ht //!< The ht object to calc the bucket count for
)
__r_warn_unused_result__
;

/*
 *
 *
 * inline implementations
 *
 *
 */

static inline size_t
ht_nbuckets(
    struct ht const* const ht
) {
    if (!ht) {
        return 0;
    }
    return CONSTPOW_TWO(ht->sizeexp);
}

#endif //__HT_H__

/**
 * @}
 */
