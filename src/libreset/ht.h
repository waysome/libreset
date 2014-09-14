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

/**
 * Type for hashtable bucket
 *
 */
struct ht_bucket {
    struct avl avl;
};

/**
 * Hashtable type
 */
struct ht {
    struct ht_bucket* buckets;
    size_t nbuckets;
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
    size_t n //!< The initial number of buckets
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
    rs_hash hash //!< The hash of the element to find
);

/**
 * Insert data into the hashtable
 *
 * @memberof ht
 *
 * @return 1 on success, else zero
 */
int
ht_insert(
    struct ht* ht, //!< The hashtable object to insert into
    void* data, //!< The data
    size_t datasize //!< The size of the data in bytes
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

#endif //__HT_H__

/**
 * @}
 */
