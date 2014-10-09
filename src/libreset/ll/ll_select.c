#include "ll/ll.h"

int
ll_select(
    struct ll const* src,
    r_predf pred,
    void* pred_etc,
    r_procf procf,
    void* dest
) {
    ll_foreach(it, src) {
        if (pred && pred(it->data, pred_etc)) {
            int retval = procf(dest, it->data);
            if (retval < 0) {
                return retval;
            }
        }
    }

    return 0;
}
