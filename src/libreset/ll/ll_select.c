#include "ll/ll.h"

int
ll_select(
    struct ll const* src,
    r_predf pred,
    void* etc,
    int (*insf)(void* elem, void* ins_etc),
    void* ins_etc
) {
    ll_foreach(it, src) {
        if (pred(it->data, etc)) {
            if (!insf(it->data, ins_etc)) {
                return 1;
            }
        }
    }

    return 0;
}
