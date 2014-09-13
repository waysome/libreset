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
