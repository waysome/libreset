#include "ll.h"

#include <stdlib.h>

struct ll*
ll_alloc(void) {
    return calloc(1, sizeof(struct ll));
}

void
ll_destroy(
    struct ll* ll
) {
}

struct ll*
ll_insert(
    struct ll* ll,
    struct ll_element* e
) {
    if (!ll || !e) {
        return NULL;
    }

    e->next = ll->head;
    ll->head = e;
    return ll;
}

struct ll*
ll_insert_data(
    struct ll* ll,
    void* data
) {
    struct ll_element* el = ll_element_alloc_new(data);
    return ll_insert(ll, el);
}

struct ll*
ll_delete(
    struct ll* ll,
    struct ll_element* del
) {
}

struct ll_element*
ll_element_alloc(void) {
}

struct ll_element*
ll_element_alloc_new(
    void* data
) {
}

void
ll_element_destroy(
    struct ll_element* el
) {
}


