#include "ll/ll.h"

size_t
ll_count(
    struct ll const* ll
) {
    if (!(ll || ll->head)) {
        return 0;
    }

    int size = 0;
    struct ll_element* iter = ll->head;

    while (iter) {
        iter = iter->next;
        size++;
    }
    return size;
}

