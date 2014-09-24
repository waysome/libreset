#include <stdlib.h>

#include "ht/ht.h"
#include "util/macros.h"
#include "ht/common.h"

#include "libreset/hash.h"

/**
 * Calculate the bucket index for a hash and a hashtable
 *
 * @return The index of the hash `hash` in the hashtable `ht`
 */
static inline size_t
bucket_index(
    struct ht const* ht,
    r_hash hash
) {
    return hash >> (BITCOUNT(hash) - ht->sizeexp);
}

struct ht*
ht_init(
    struct ht* ht,
    size_t n
) {
    if (ht) {
        ht->buckets = calloc(CONSTPOW_TWO(n), sizeof(*ht->buckets));
        ht->sizeexp = n;
        ht_dbg("Allocated %zi buckets for %p", CONSTPOW_TWO(n), (void*) ht);
    }

    return ht;
}

void
ht_destroy(
    struct ht* ht,
    struct r_set_cfg const* cfg
) {
    if (ht) {
        size_t i = ht_nbuckets(ht);
        ht_dbg("Destroying %p with %zi buckets", (void*) ht, i);
        while (i--) {
            avl_destroy(&ht->buckets[i].avl, cfg);
        }
        free(ht->buckets);
    }
}

int
ht_del(
    struct ht* ht,
    void const* cmp,
    struct r_set_cfg const* cfg
) {
    r_hash hash = cfg->hashf(cmp);
    size_t i = bucket_index(ht, hash);
    ht_dbg("Deleting element with hash %zi in bucket %zi", hash, i);
    return avl_del(&ht->buckets[i].avl, hash, cmp, cfg);
}

void*
ht_find(
    struct ht const* ht,
    void const* cmp,
    struct r_set_cfg const* cfg
) {
    r_hash hash = cfg->hashf(cmp);
    size_t i = bucket_index(ht, hash);
    ht_dbg("Finding element with hash %zi in bucket %zi", hash, i);
    return avl_find(&ht->buckets[i].avl, hash, cmp, cfg);
}

unsigned int
ht_ndel(
    struct ht* ht,
    r_predf pred,
    void* etc,
    struct r_set_cfg const* cfg
) {
    size_t i;
    unsigned int sum = 0;

    for (i = 0; i < ht_nbuckets(ht); i++) {
        sum += avl_ndel(&ht->buckets[i].avl, pred, etc, cfg);
    }

    return sum;
}

int
ht_insert(
    struct ht* ht,
    void* data,
    struct r_set_cfg const* cfg
) {
    r_hash hash = cfg->hashf(data);

    // this is equivalent to hash / 2^(BITCOUNT(hash) - ht->sizeexp) due to the
    // right shift
    size_t i = bucket_index(ht, hash);
    return avl_insert(&ht->buckets[i].avl, hash, data, cfg);
}
