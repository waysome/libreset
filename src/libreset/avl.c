#include "libreset/avl.h"

#include <stdlib.h>

#include "libreset/hash.h"
#include "libreset/util/macros.h"

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
    int (*pred)(void* const, void*), //!< predicate function
    void* etc, //!< additional argument for the predicate function
    struct avl_el** parent_dest, //!< destination of the ptr to the parent
    struct avl_el** found_dest //!< destination of the ptr to the element
) {
    struct avl_el* next;
    int predret = pred(root, etc);

    if (root == NULL) {
        return 0;
    }

    if (predret == 0) {
        if (parent_dest) {
            *parent_dest = NULL;
        }
        if (found_dest) {
            *found_dest = root;
        }
        return 1;
    } else if (predret == -1) {
        next = root->l;
    } else {
        next = root->r;
    }

    if (parent_dest) {
        *parent_dest = root;
    }
    return find_element_with_parent(next, pred, etc, parent_dest, found_dest);
}

/**
 * Perform a rotation between a root node and its left child
 *
 * @warning Call only if root node has left child
 *
 * @return The new root
 */
static struct avl_el*
single_rotate_with_left(struct avl_el* root) {
    struct avl_el* k = NULL;

    k = root->l;
    root->l = k->r;
    k->r = root;

    root->height = MAX(avl_height(root->l), avl_height(root->r)) + 1;
    k->height    = MAX(avl_height(k->l), root->height) + 1;

    return k;
}

/**
 * Perform a rotation between a root node and its right child
 *
 * @warning Call only if root node has right child
 *
 * @return The new root
 */
static struct avl_el*
single_rotate_with_right(struct avl_el* root) {
    struct avl_el* k = NULL;

    k = root->r;
    root->r = k->l;
    k->l = root;

    root->height = MAX(avl_height(root->l), avl_height(root->r)) + 1;
    k->height    = MAX(avl_height(k->r), root->height) + 1;

    return k;
}

/**
 * Perform left-right double rotation
 *
 * @warning Call only if root node has a left child and this one has a right
 * child
 *
 * @return The new root
 */
static inline struct avl_el*
double_rotate_with_left(struct avl_el* root) {
    root->l = single_rotate_with_right(root->l);
    return single_rotate_with_left(root);
}

/**
 * Perform right-left double rotation
 *
 * @warning Call only if root node has a right child and this one has a left
 * child
 *
 * @return The new root
 */
static inline struct avl_el*
double_rotate_with_right(struct avl_el* root) {
    root->r = single_rotate_with_left(root->r);
    return single_rotate_with_right(root);
}

/**
 * Add a node to a tree
 *
 * @return the new root node
 */
static struct avl_el*
insert(struct avl_el* new, struct avl_el* root) {
    signed int cmpres = new->hash == root->hash ? 0 : (new->hash > root->hash ? -1 :
                                                       1);

    if (root == NULL) {
        return new;
    } else if (cmpres == 1) {
        root->l = insert(new, root->l);
        if (avl_height(root->l) - avl_height(root->r) == 2) {
            if (new->hash != root->l->hash) {
                root = single_rotate_with_left(root);
            } else {
                root = double_rotate_with_left(root);
            }
        }
    } else if (cmpres == -1) {
        root->r = insert(new, root->r);
        if (avl_height(root->r) - avl_height(root->l) == 2) {
            if (new->hash != root->r->hash) {
                root = single_rotate_with_right(root);
            } else {
                root = double_rotate_with_right(root);
            }
        }
    } else {
        // TODO: `new` is already in the tree
    }

    root->height = MAX(avl_height(root->l), avl_height(root->r)) + 1;
    return root;
}

struct avl_el*
avl_alloc(void) {
    return avl_add(NULL, NULL);
}

int
avl_destroy(
    struct avl_el* root
) {
    if (root->l) {
        avl_destroy(root->l);
        root->l = NULL;
    }

    if (root->r) {
        avl_destroy(root->r);
        root->r = NULL;
    }

    root->data = NULL;
    free(root);

    return 1;
}

struct avl_el*
avl_add(
    struct avl_el* root,
    void* const data
) {
    struct avl_el* new = calloc(1, sizeof(*new));

    if (new) {
        new->data = data;
        new->l = NULL;
        new->r = NULL;

        if (root) {
            // TODO: Adding the element (new) to the tree (root)
        }
    }

    return new;
}

struct avl_el*
avl_del(
    struct avl_el* root,
    int (*pred)(void* const, void*),
    void* etc
) {
    struct avl_el* parent;
    struct avl_el* found;
    int f = find_element_with_parent(root, pred, etc, &parent, &found);

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
    int (*pred)(void* const, void*),
    void* etc
) {
    struct avl_el* found;
    find_element_with_parent(root, pred, etc, NULL, &found);
    return found;
}

struct avl_el*
avl_unlink(
    struct avl_el* root,
    int (*pred)(void* const, void*),
    void* etc
) {
    struct avl_el* parent;
    struct avl_el* found;
    int f = find_element_with_parent(root, pred, etc, &parent, &found);

    if (f) {
        parent->l = NULL;
        parent->r = NULL;

        // TODO: resort parent tree (root)
    }

    return found;
}
