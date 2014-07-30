#include "libreset/avl.h"

#include <stdlib.h>

struct avl_el*
avl_alloc(void) {
    return avl_add(NULL, NULL, NULL);
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
    int (*cmp)(void* const, void* const),
    void* const d
) {
    struct avl_el* new = calloc(1, sizeof(*new));

    if (new) {
        new->d = d;
        new->l = NULL;
        new->r = NULL;

        if (root) {
            // TODO: Adding the element (new) to the tree (root)
        }
    }

    return new;
}

int
avl_del(
    struct avl_el* root,
    int (*cmp)(void* const, void* const, void*),
    void* etc
) {
    // TODO
    return 0;
}

struct avl_el*
avl_find(
    struct avl_el* root,
    int (*cmp)(void* const, void* const, void*),
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
