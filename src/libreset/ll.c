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
    struct ll_element* iter = ll->head;
    struct ll_element* next;

    while (iter) {
        next = iter->next;
        free(iter);
        iter = next;
    }
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
    struct ll_element* iter;
    struct ll_element* last;

    for (iter = ll->head; iter; iter = iter->next) {
        if (iter == del) {
            last->next = iter->next;
            free(iter);
            break;
        }

        last = iter;
    }

    return ll;
}

struct ll_element*
ll_element_alloc(void) {
    return calloc(1, sizeof(struct ll_element));
}

struct ll_element*
ll_element_alloc_new(
    void* data
) {
    struct ll_element* el = ll_element_alloc();
    if (el) {
        el->data = data;
    }
    return el;
}

void
ll_element_destroy(
    struct ll_element* el
) {
    free(el);
}

