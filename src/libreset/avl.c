#include "libreset/avl.h"

struct avl_el*
avl_alloc(void) {
    // TODO
    return NULL;
}

int
avl_destroy(
    struct avl_el* root
) {
    // TODO
    return 0;
}

struct avl_el*
avl_add(
    struct avl_el* root,
    void* const d
) {
    // TODO
    return NULL;
}

int
avl_del(
    struct avl_el* root,
    int (*cmp)(void* const, void*),
    void* etc
) {
    // TODO
    return 0;
}

struct avl_el*
avl_find(
    struct avl_el* root,
    int (*cmp)(void* const, void*),
    void* etc
) {
    // TODO
    return NULL;
}

struct avl_el*
avl_unlink(
    struct avl_el* root,
    int (*cmp)(void* const, void*),
    void* etc
) {
    // TODO
    return NULL;
}
