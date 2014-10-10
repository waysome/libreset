#include "ht/ht.h"

int
ht_union(
    struct ht* dest,
    struct ht const* src,
    struct r_set_cfg const* cfg
) {
    size_t dest_b = ht_nbuckets(dest);
    size_t bucket_ration = ht_nbuckets(src) / dest_b;
    //!< @todo: implement case where bucket_ratio is 0

    // iterate over all the destination buckets
    while (dest_b--) {
        struct avl* avl_dest = &dest->buckets[dest_b].avl;

        // iterate over all the source buckets for a destination
        size_t b = bucket_ration;
        size_t off = dest_b * bucket_ration;
        while (b--) {

            // perform the union merge for the bucket
            struct avl* avl_src = &src->buckets[off + b].avl;
            int retval = avl_union(avl_dest, avl_src, cfg);
            if (retval < 0) {
                return retval;
            }
        }
    }

    return 0;
}
