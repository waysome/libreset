#include "ll.h"

int
ll_is_subset(
    struct ll const* ll_a,
    struct ll const* ll_b,
    struct r_set_cfg const* cfg
) {
    if (!ll_a) {
        return 1;
    }

    ll_foreach(it, ll_a) {
        if (!ll_find(ll_b, it->data, cfg)) {
            return 0;
        }
    }

    return 1;
}
