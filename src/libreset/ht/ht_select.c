#include "ht/ht.h"

int
ht_select(
    struct ht* dest,
    struct ht const* src,
    r_predf pred,
    void* etc,
    struct r_set_cfg* const cfg
) {
    size_t i;
    size_t nbucks = ht_nbuckets(src);

    struct insert_etc_data etc_data = { .ht = dest, .cfg = cfg };

    for (i = 0; i < nbucks; i++) {
        if (!avl_select(&src->buckets[i].avl, pred, etc, insert, &etc_data)) {
            return 1;
        }
    }

    return 0;
}
