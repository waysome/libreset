#include "ll/ll.h"

unsigned int
ll_count(
    struct ll const* ll
) {
    if(!(ll || ll->head)) {
        return 0;
    }

    int size = 0;
    struct ll_element* iter = ll->head;

    while(iter) {
        iter = iter->next;
        size++;
    }
    return size;
}

