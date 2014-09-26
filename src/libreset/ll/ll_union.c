#include "ll/ll.h"

int
ll_union(
    struct ll* dest,
    struct ll const* src,
    struct r_set_cfg const* cfg
) {
    ll_foreach(it, src) {
        if (ll_insert(dest, it->data, cfg) == 0) {
            return 1;
        }
    }

    return 0;
}
