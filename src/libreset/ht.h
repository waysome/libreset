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
#include "avl.h"
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
);

/**
 * Destroy a struct ht object
 *
 * @memberof ht
 */
void
ht_destroy(
    struct ht* ht //!< The hashtable object to destory
);

/**
 * Find an element inside the hashtable by its hash
 *
 * @memberof ht
 *
 * @return the found element or NULL on failure
 */
struct ht_bucket*
ht_find(
    struct ht* ht, //!< The hashtable object to search in
    rs_hash hash, //!< The hash of the element to find
    void* cmp, //!< Element to compare against
    struct r_set_cfg* cfg //!< type information provided by user
);

/**
 * Insert data into the hashtable
 *
 * @memberof ht
 *
 * @return the hash of the inserted element
 */
rs_hash
ht_insert(
    struct ht* ht, //!< The hashtable object to insert into
    void* data, //!< The data
);

/**
 * Delete something from the hashtable by hash
 *
 * @memberof ht
 *
 * @return the deleted element or NULL if not found
 */
struct element*
ht_del(
    struct ht* ht, //!< The hashtable object to delete from
    rs_hash hash
);

/**
 * Helper to calculate the actual bucket count of the hashtable
 *
 * @memberof ht
 *
 * @return The number of buckets in `ht` or zero on failure.
 */
static inline size_t
ht_nbuckets(
    struct ht* ht //!< The hashtable object to calculate the bucket count for
);

/*
 *
 *
 * inline implementations
 *
 *
 */

static inline size_t
ht_nbuckets(
    struct ht* ht
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
