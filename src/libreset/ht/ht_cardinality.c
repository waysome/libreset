#include "ht.h"

int
ht_cardinality(
    struct ht const* ht
) {
    size_t i = ht_nbuckets(ht);
    int card = 0;

    while(i--) {
        card += avl_cardinality(&ht->buckets[i].avl);
    }

    return card;
}
