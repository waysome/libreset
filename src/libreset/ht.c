#include <stdlib.h>

#include "ht.h"

struct ht*
ht_init(
    struct ht* ht,
    size_t n
) {
    if (ht) {
        ht->buckets = calloc(n, sizeof(*ht->buckets));
        ht->nbuckets = n;
    }

    return ht;
}

void
ht_destroy(
    struct ht* ht
) {
    if (ht) {
        for (; ht->nbuckets > 0; ht->nbuckets--) {
            avl_destroy(&ht->buckets[ht->nbuckets - 1].avl);
        }
        free(ht->buckets);
        free(ht);
    }
}
