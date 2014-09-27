#include "ht/ht.h"

int
ht_select(
    struct ht* src,
    r_predf pred,
    void* pred_etc,
    r_procf procf,
    void* dest
) {
    size_t i = ht_nbuckets(src);

    while (i--) {
        struct ht_bucket* buck = &src->buckets[i];
        int retval = avl_select(&buck->avl, pred, pred_etc, procf, dest);
        if (retval < 0) {
            return retval;
        }
    }

    return 0;
}

