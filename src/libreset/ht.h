#ifndef __HT_H__
#define __HT_H__

#include "libreset/hash.h"
#include "libreset/element.h"
#include "libreset/avl.h"

/**
 * Type for hashtable bucket
 *
 */
struct rs_ht_bucket {
    struct avl avl;
};

/**
 * Hashtable type
 */
struct rs_ht {
    struct rs_ht_bucket* buckets;
    size_t nbuckets;
};

/**
 * Initialize a struct rs_ht object
 *
 * @return the pointer to the passed struct rs_ht object, NULL on failure
 */
struct rs_ht*
rs_ht_init(
    struct rs_ht* ht, //!< The hashtable object to initialize
    size_t n //!< The initial number of buckets
);

/**
 * Destroy a struct rs_ht object
 */
void
rs_ht_destroy(
    struct rs_ht* ht //!< The hashtable object to destory
);

/**
 * Find an element inside the hashtable by its hash
 *
 * @return the found element or NULL on failure
 */
struct rs_element*
rs_ht_find(
    struct rs_ht* ht, //!< The hashtable object to search in
    rs_hash hash //!< The hash of the element to find
);

/**
 * Insert data into the hashtable
 *
 * @return 1 on success, else zero
 */
int
rs_ht_insert(
    struct rs_ht* ht, //!< The hashtable object to insert into
    void* data, //!< The data
    size_t datasize //!< The size of the data in bytes
);

/**
 * Delete something from the hashtable by hash
 *
 * @return the deleted element or NULL if not found
 */
struct rs_element*
rs_ht_del(
    struct rs_ht* ht, //!< The hashtable object to delete from
    rs_hash hash
);

#endif //__HT_H__
