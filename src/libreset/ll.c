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
ll_append(
    struct ll* ll,
    struct ll_element* e
) {
}

struct ll*
ll_append_data(
    struct ll* ll,
    void* data
) {
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



