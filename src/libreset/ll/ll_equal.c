#include "ll/ll.h"

int
ll_equal(
    struct ll const* lla,
    struct ll const* llb,
    struct r_set_cfg const* cfg //!< Type information provided by the user
) {
    if (lla == llb) {
        return 1;
    }

    if ((lla == NULL) ^ (llb == NULL)) {
        return 0;
    }

    ll_foreach(it_a, lla) {
        if (NULL == ll_find(llb, it_a->data, cfg)) {
            return 0;
        }
    }

    return 1;
}
