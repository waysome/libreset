#ifndef __HT_H__
#define __HT_H__

#include "libreset/hash.h"
#include "libreset/element.h"
#include "libreset/avl.h"

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
 * @return the pointer to the passed struct ht object, NULL on failure
 */
struct ht*
ht_init(
    struct ht* ht, //!< The hashtable object to initialize
    size_t n //!< The initial number of buckets
);

/**
 * Destroy a struct ht object
 */
void
ht_destroy(
    struct ht* ht //!< The hashtable object to destory
);

/**
 * Find an element inside the hashtable by its hash
 *
 * @return the found element or NULL on failure
 */
struct element*
ht_find(
    struct ht* ht, //!< The hashtable object to search in
    hash hash //!< The hash of the element to find
);

/**
 * Insert data into the hashtable
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
 * @return the deleted element or NULL if not found
 */
struct element*
ht_del(
    struct ht* ht, //!< The hashtable object to delete from
    hash hash
);

#endif //__HT_H__
