
#include "ht/ht.h"

int
ht_is_subset(
    struct ht const* ht_a,
    struct ht const* ht_b,
    struct r_set_cfg const* cfg
) {
    // ht_a cannot be a subset of ht_b if it is larger than ht_b
    if (ht_cardinality(ht_a) > ht_cardinality(ht_b)) {
        return 0;
    }

    if (ht_a->sizeexp > ht_b->sizeexp) {
        int i = ht_nbuckets(ht_b);
        int denom_buckets = 1 << (ht_a->sizeexp - ht_b->sizeexp);
        while (i--) {
            int j = denom_buckets;
            while(j--) {
                if (!avl_is_subset(&ht_a->buckets[denom_buckets * j+i].avl,
                        &ht_b->buckets[i].avl, cfg)) {
                    return 0;
                }
            }
        }
    } else if (ht_a->sizeexp < ht_b->sizeexp) {
        // To be implemented
    } else {
        int i = ht_nbuckets(ht_b);
        while (i--) {
            if (!avl_is_subset(&ht_a->buckets[i].avl,
                        &ht_b->buckets[i].avl, cfg)) {
                return 0;
            }
        }
    }

    return 1;
}

