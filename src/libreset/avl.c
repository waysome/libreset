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
 * The value of `*found_dest` is a pointer to the found node, after the function
 * returns
 *
 * The value of `*parent_dest` is a pointer to the parent of the found node,
 * after the function returns.
 *
 * @warning `pred` shouldn't be NULL
 *
 * @note `found_dest` can be NULL
 *
 * @note `parent_dest` can be NULL
 *
 * @note It doesn't make sense that `found_dest` _and_ `parent_dest` are NULL.
 *
 * @note Does pre-order search.
 *
 * @parblock
 *
 * @b Approach:
 *
 *  1.  Check if the passed `struct avl_el` exists, return 0 if not, do
 *      not touch anything.
 *
 *  2.  If the root node is the node we search:
 *
 *      2.1.    Set the `*parent_dest` to NULL, as we assume this is
 *              the first recursion step
 *
 *      2.2.    If the `found_dest` exists, set it.
 *
 *      2.3.    return 1, we found it
 *
 *  3. Check the left and the right subtree by doing a recursion step.
 *
 *      Found:  If the recursion step has found the node we look for, set
 *              `*parent_dest` accordingly and only if it is not set
 *              yet.
 *              This prevents further (upper) recursion steps from
 *              re-setting it, which would result the root node of the
 *              tree we operate on to be saved as the parent node, which
 *              is wrong.
 *              return 1 afterwards, we found it.
 *
 *      Not:    The node we look for is not in the tree. Ensure the
 *              `*parent_node` is NULL and return 0, nothing found.
 * @endparblock
 *
 * @return 1 if the element is found, else 0
 */
static int
find_element_with_parent(
    struct avl_el* root, //!< root of the tree
    rs_predicate_function pred, //!< predicate function
    void* etc, //!< additional argument for the predicate function
    struct avl_el** parent_dest, //!< destination of the ptr to the parent
    struct avl_el** found_dest //!< destination of the ptr to the element
) {

    if (!root) {
        return 0;
    }

    if (pred(root, etc) && found_dest) {
        if (parent_dest) {
            *parent_dest = NULL;
        }
        *found_dest = root;
        return 1;
    }

    if (find_element_with_parent(root->l, pred, etc, parent_dest, found_dest) ||
        find_element_with_parent(root->r, pred, etc, parent_dest, found_dest)) {
        /**
         * Element FOUND in one of the subtrees tree
         *
         * - found_dest was set by find_element_with_parent() call
         * - set parent_dest if and only if it was not set before (by a
         *   recursion step)
         */
        if (parent_dest && *parent_dest != NULL) {
            *parent_dest = root;
        }

        return 1;
    }
    /* element NOT FOUND in one of the subtrees */

    /**
     * ensure the parent_dest is NULL
     */
    if (parent_dest) {
        *parent_dest = NULL;
    }

    return 0;
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
 * @invariant \f$ -1 \leq H(n) \leq +1 \f$, Where \f$ H(n) \f$ is the
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
 * Find a node by its hash
 *
 * @warning `root` may not be NULL
 *
 * @return Pointer to a node with the hash `hash` or NULL if none is found
 *
 * @todo Check relation
 */
static struct avl_el*
find_node_with_hash(
    struct avl_el* root,    //!< The node where to begin to search in
    rs_hash h               //!< The hash to search for
) {
    if (root->hash == h) {
        return root;
    }

    if (root->hash >= h) {
        if (root->l) {
            return find_node_with_hash(root->l, h);
        }
    } else {
        if (root->r) {
            return find_node_with_hash(root->r, h);
        }
    }

    return NULL;
}

/**
 * Insert a node into a tree
 *
 * Does rotations and updates the root node if neccessary
 *
 * @warning Updates `root` if neccessary
 */
static void
insert_node_into_tree(
    struct avl_el* node, //!< node to insert
    struct avl_el** root //!< Ptr to root node
) {
}

/**
 * Add a data element to a avl tree
 *
 * @return Ptr to the struct avl_el node where the data was inserted into or
 *         NULL on failure
 */
static struct avl_el*
insert(
    void* data,             //!< The data ptr to insert
    rs_hash hash,           //!< The hash of the data to insert
    struct avl_el** root    //!< root node ptr source and destination
) {
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
    void* const data,
    rs_hash hash
) {
    return insert(data, hash, &avl->root);
}

/**
 * @bug When deleting the root node, everything crashes.
 */
struct avl_el*
avl_del(
    struct avl* avl,
    rs_predicate_function pred,
    void* etc
) {
    struct avl_el* parent;
    struct avl_el* found;
    int f = find_element_with_parent(avl->root, pred, etc, &parent, &found);

    if (!f) {
        return NULL;
    }

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

    /**
     * @todo rebalancing, with rotations if neccessary.
     */

    return avl->root;
}

struct avl_el*
avl_find(
    struct avl* avl,
    rs_predicate_function pred,
    void* etc
) {
    struct avl_el* found;
    find_element_with_parent(avl->root, pred, etc, NULL, &found);
    return found;
}
