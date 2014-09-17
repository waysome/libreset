#include <stdlib.h>

#include "ht.h"
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
    struct ht* ht
) {
    if (ht) {
        size_t i = ht_nbuckets(ht);
        while (i--) {
            // TODO: add cfg
            avl_destroy(&ht->buckets[i].avl, NULL);
        }
        free(ht->buckets);
        free(ht);
    }
}

