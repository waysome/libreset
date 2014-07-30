#include "libreset/avl.h"

#include <stdlib.h>

/**
 * Find an element and its parent
 *
 * Helper function to find an element and its parent, for simplification of
 * avl_find() and avl_del() functions.
 *
 * If the element is found, the found_dest pointer is set and the parent_dest
 * pointer is set. They can be NULL, but it is clearly unreasonable to set them
 * both to NULL.
 *
 * The parent_dest and found_dest are always safe to return without check.
 *
 * @return 1 if the element is found, else 0
 */
static int
find_element_with_parent(
    struct avl_el* root, //!< root of the tree
    int (*cmp)(void* const, void* const, void*), //!< comparator function
    void* etc, //!< additional argument for the comperator function
    struct avl_el** parent_dest, //!< destination of the ptr to the parent
    struct avl_el** found_dest //!< destination of the ptr to the element
) {
    return 0;
}

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
    struct avl_el* parent;
    struct avl_el* found;
    int f = find_element_with_parent(root, cmp, etc, &parent, &found);

    if (f) {

        // TODO: reorganize tree if neccessary, else:
        if (found->l == NULL && found->r == NULL) {
            if (parent->l == found) {
                parent->l = NULL;
            } else {
                parent->r = NULL;
            }
        }

        free(found);
    }

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
