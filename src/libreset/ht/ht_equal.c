#include "ht/ht.h"

int
ht_equal(
    struct ht const* ht_a,
    struct ht const* ht_b,
    struct r_set_cfg const* cfg
) {
    // Two sets who have different amount of elements are never equal
    if (ht_cardinality(ht_a) != ht_cardinality(ht_b)) {
        return 0;
    }

    if (ht_a->sizeexp != ht_b->sizeexp) {
        // We switch the pointers so they are always ordered in the same way
        if (ht_a->sizeexp < ht_b->sizeexp) {
            struct ht const* ht_tmp = ht_a;
            ht_a = ht_b;
            ht_b = ht_tmp;
        }
        int denom_buckets = 1 << (ht_a->sizeexp - ht_b->sizeexp);

        // We verify that all the buckets have the same cardinality with their
        // corresponding one
        int i = ht_nbuckets(ht_b);
        while (i--) {
            size_t size = 0;
            int j = denom_buckets;
            while (j--) {
                size += avl_cardinality(&ht_a->buckets[denom_buckets*i +j].avl);
            }
            if (size != avl_cardinality(&ht_b->buckets[i].avl)) {
                return 0;
            }
        }

        // All the avls have the same cardinality, now we check if each element
        // is present in each other bucket
        i = ht_nbuckets(ht_b);
        while (i--) {
            int j = denom_buckets;
            while (j--) {
                if (!avl_is_subset(&ht_a->buckets[denom_buckets*i +j].avl,
                        &ht_b->buckets[i].avl, cfg)) {
                    return 0;
                }
            }
        }
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

