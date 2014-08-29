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
 * Get the higher child of a node
 *
 * @note NULL-safe. If either of the children of the root node is NULL, the
 * other child is returned.
 *
 * @warning Returns NULL if both childs are NULL.
 *
 * @return The higher child of a node, the right one if both are of equal height
 */
static inline struct avl_el*
find_higher_child(struct avl_el* root) {
    if (!root->l) {
        return root->r;
    }
    if (!root->r) {
        return root->l;
    }

    if (root->l->height > root->r->height) {
        return root->l;
    } else {
        return root->r;
    }
}

/**
 * Get the height difference between two nodes
 */
static inline signed int
height_diff(
    struct avl_el* a, //!< The first node
    struct avl_el* b  //!< The first node
) {
    int h1 = (a == NULL ? 0 : a->height);
    int h2 = (b == NULL ? 0 : b->height);
    return ((h1 >= h2) ? (h1 - h2) : (h2 - h1));
}

/**
 * Recompute the height for a subtree
 *
 * @invariant \f$ -1 \leqslant H(n) \leqslant +1 \f$, Where \f$ H(n) \f$ is the
 * height of a node.
 *
 * @note NULL-safe, returns zero
 *
 * @return The height of the passed node or zero if the node is NULL
 */
static inline int
recompute_height(struct avl_el* root) {
    if (!root) {
        return 0;
    }

    if (!root->l && !root->r) {
        root->height = 0;
    } else {
        root->height = - recompute_height(root->l) + recompute_height(root->r);
    }

    return root->height;
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
 * Find leftmost node of the passed node
 *
 * @note Uses `p` as temporary variable while retreiving the leftmost node.
 *
 * @note Ensures `p` to be NULL if the leftmost node has no parent
 *
 * @return The leftmost node starting from the passed node
 */
static inline struct avl_el*
        __returns_nonnull__
find_leftmost_node_with_parent(
    struct avl_el* root, //!< The root
    struct avl_el** p    //!< Destination for parent of leftmost node
) {
    if (root) {
        while (root->l) {
            *p = root;
            root = root->l;
        }
    } else {
        *p = NULL;
    }
    return root;
}

/**
 * Add a node to a tree
 *
 * @return the new root node
 */
static struct avl_el*
insert(struct avl_el* new, struct avl_el** root) {
    signed int cmpres = new->hash == (*root)->hash ? 0 : (new->hash >
                                                          (*root)->hash ? -1 : 1);

    if (*root == NULL) {
        return new;
    } else if (cmpres == 1) {
        (*root)->l = insert(new, &(*root)->l);
        if (avl_height((*root)->l) - avl_height((*root)->r) == 2) {
            if (new->hash != (*root)->l->hash) {
                *root = single_rotate_with_left(*root);
            } else {
                *root = double_rotate_with_left(*root);
            }
        }
    } else if (cmpres == -1) {
        (*root)->r = insert(new, &(*root)->r);
        if (avl_height((*root)->r) - avl_height((*root)->l) == 2) {
            if (new->hash != (*root)->r->hash) {
                *root = single_rotate_with_right(*root);
            } else {
                *root = double_rotate_with_right(*root);
            }
        }
    } else {
        // TODO: `new` is already in the tree
    }

    (*root)->height = MAX(avl_height((*root)->l), avl_height((*root)->r)) + 1;
    return *root;
}

static void
destroy_subtree(
    struct avl_el* root
) {
    if (root->l) {
        destroy_subtree(root->l);
        root->l = NULL;
    }

    if (root->r) {
        destroy_subtree(root->r);
        root->r = NULL;
    }

    root->data = NULL;
    free(root);
}

struct avl*
avl_alloc(void) {
    return calloc(1, sizeof(struct avl));
}

int
avl_destroy(
    struct avl* avl
) {
    if (avl && avl->root) {
        destroy_subtree(avl->root);
        free(avl);
        return 1;
    } else {
        return 0;
    }
}

struct avl_el*
avl_add(
    struct avl* avl,
    void* const data
) {
    struct avl_el* new = calloc(1, sizeof(*new));

    if (new) {
        new->data = data;
        new->l = NULL;
        new->r = NULL;

        new = insert(new, &avl->root);
    }

    return new;
}

/**
 * @bug When deleting the root node, everything crashes.
 */
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

        /**
         * Three possible cases:
         *
         * 1) `found` has no children. Delete `found`
         * 2) `found` has one child. Delete `found` and link `parent` to the
         *     child of `found`
         * 3) `found` has two children:
         *      3.1) find `found`s successor `z` (which is the leftmost node in
         *           the right subtree of `found`)
         *      3.2) replace `found` with its successor `z`
         *      3.3) delete `z`
         *           Note: `z` does not have a left child.
         *           Note: `z` has at most one child, we can use case (1) or (2)
         *                 to delete `z`
         */

        if (!found->l && !found->r) {
            /* case (1) */
            if (parent->l == found) {
                parent->l = NULL;
            } else { /* parent->r == found */
                parent->r = NULL;
            }
        } else if ((found->l && !found->r) || (!found->l && found->r)) {
            /* case (2) */
            struct avl_el* link = (found->l) ? found->l : found->r;

            if (parent->l == found) {
                parent->l = link;
            } else { /* parent->r == found */
                parent->r = link;
            }
        } else { /* `found` has two children */
            /* case (3) */

            /*
             * Find successor of `found`, which is called `lm` here (for
             * 'leftmost'). Also find its parent.
             *
             * Store its child, too, so we have it stored when replacing `found`
             * with `lm`.
             */
            struct avl_el* lm_parent;
            struct avl_el* lm = find_leftmost_node_with_parent(found, &lm_parent);
            struct avl_el* lm_child = (lm->r ? lm->r : NULL);

            /*
             * replace `lm` with `found`
             */
            if (parent->l == found) {
                parent->l = lm;
            } else {
                parent->r = lm;
            }

            /*
             * Replace `lm` left and right ptr and its height with the data from
             * `found`.
             */
            lm->r       = parent->r;
            lm->l       = parent->l;
            lm->height  = parent->height;

            /*
             * `lm` is always left node after `lm_parent`
             *
             * Set possible child of `lm` to the left node after the parent of
             * `lm`, `lm_parent`
             */
            lm_parent->l = (lm_child ? lm_child : NULL);
        }

        /*
         * Nothing should now point to `found` and nothing should dangling
         * around, all ptrs from `found` to somewhere are safed.
         */
        free(found);
    }

    /**
     * @todo rebalancing, with rotations if neccessary.
     */

    return root;
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
