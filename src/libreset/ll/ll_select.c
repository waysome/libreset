#include "ll/ll.h"

int
ll_select(
    struct ll const* src,
    r_predf pred,
    void* etc,
    int (*insf)(void* elem, void* insetc),
    void* insetc
) {
    ll_foreach(it, src) {
        if (pred(it->data, etc)) {
            if (!insf(it->data, insetc)) {
                return 1;
            }
        }
    }

    return 0;
}
