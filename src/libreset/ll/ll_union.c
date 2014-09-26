#include <errno.h>
#include "ll/ll.h"

int
ll_union(
    struct ll* dest,
    struct ll const* src,
    struct r_set_cfg const* cfg
) {
    ll_foreach(it, src) {
        int res = ll_insert(dest, it->data, cfg);
        if (res != 0) {
            return res;
        }
    }

    return 0;
}
