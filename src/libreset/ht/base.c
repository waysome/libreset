#include <stdlib.h>

#include "ht/ht.h"
#include "util/macros.h"

struct ht*
ht_init(
    struct ht* ht,
    size_t n
) {
    if (ht) {
        ht->buckets = calloc(CONSTPOW_TWO(n), sizeof(*ht->buckets));
        ht->sizeexp = n;
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
    rs_hash hash = cfg->hashf(cmp);
    size_t i = hash >> (sizeof(rs_hash) - ht->sizeexp);
    return avl_del(&ht->buckets[i].avl, hash, cmp, cfg);
}

void*
ht_find(
    struct ht const* ht,
    void const* cmp,
    struct r_set_cfg const* cfg
) {
    rs_hash hash = cfg->hashf(cmp);
    size_t i = hash >> (sizeof(hash) - ht->sizeexp);
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
    rs_hash hash = cfg->hashf(data);
    size_t i = hash >> (sizeof(hash) - ht->sizeexp);
    return avl_insert(&ht->buckets[i].avl, hash, data, cfg);
}
