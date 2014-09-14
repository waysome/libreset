#include <stdlib.h>

#include "libreset/hash.h"

#include "util/likely.h"
#include "util/macros.h"

#include "ll.h"
#include "avl.h"
#include "util/macros.h"


/*
 *
 *
 * forward declarations
 *
 *
 */

static void
destroy_subtree(
    struct avl_el* node //!< A node to destroy
);

/**
 * Rebalance a subtree
 *
 * This function will recursively rebalance a subtree.
 * It uses the metadata of a subtree's root node to check whether the subtree
 * already is balanced. This way, unnecessary recursions are omitted.
 *
 * @return new root
 */
static struct avl_el*
rebalance_subtree(
    struct avl_el* root //!< The root of the subtree to rebalance
);

/**
 * Rotate a node counter-clockwise
 *
 * @return new root or NULL, if the rotation could not be performed
 */
static struct avl_el*
rotate_left(
    struct avl_el* node //!< The node to rotate
);

/**
 * Rotate a node clockwise
 *
 * @return new root or NULL, if the rotation could not be performed
 */
static struct avl_el*
rotate_right(
    struct avl_el* node //!< The node to rotate
);

/**
 * Regenerate a node's height and node_cnt
 *
 * @return void
 * @warning This function will crash when being passed NULL.
 *
 * This function regenerates the buffered metadata of a node with a depth of 1.
 * The function does not recurse but only takes into account the metadata of
 * direct children.
 */
static void
regen_metadata(
    struct avl_el* node //!< The node to regenerate
);

/**
 * Create a new struct avl_el object
 *
 * @return Ptr to the new struct avl_el object or NULL on failure
 */
static struct avl_el*
new_avl_el(
    rs_hash h //!< The hash for the new struct avl_el object
);

/**
 * Insert an struct avl_el object into the avl tree
 *
 * @pre An element with the hash of the to-insert element (`el`) is not in the
 *      tree `root` points to.
 *
 * @return The new root element
 */
static struct avl_el*
insert_element_into_tree(
    struct avl_el* el, //!< The element to insert
    struct avl_el** root //!< The root element of the tree where to insert
);

/*
 *
 *
 * interface implementation
 *
 *
 */

struct avl*
avl_alloc(void) {
    return calloc(1, sizeof(struct avl));
}

int
avl_destroy(
    struct avl* avl //!< The avl tree
) {
    if (avl && avl->root) {
        destroy_subtree(avl->root);
    }
    return 1;
}

struct avl_el*
avl_find(
    struct avl* avl,
    rs_hash hash
) {
    struct avl_el* iter = avl->root;

    while (iter && iter->hash != hash) {
        if (iter->hash > hash) {
            iter = iter->l;
        } else {
            iter = iter->r;
        }
    }

    return iter;
}

struct avl_el*
avl_add(
    struct avl* avl, //!< The avl tree where to insert
    void* const d, //!< The data element
    rs_hash hash //!< The hash for the data element
) {
    struct avl_el* element = avl_find(avl, hash);

    if (element) {
        ll_insert_data(&element->ll, d);
    } else {
        element = new_avl_el(hash);
        if (element) {
            ll_insert_data(&element->ll, d);
            insert_element_into_tree(element, &avl->root);
            rebalance_subtree(avl->root);
        } else {
            /* Out of memory */
        }
    }

    return element;
}

/*
 *
 *
 * implementation of internal functions
 *
 *
 */

static struct avl_el*
new_avl_el(
    rs_hash h
) {
    struct avl_el* el = calloc(1, sizeof(*el));
    if (el) {
        el->hash = h;
    }
    return el;
}

static struct avl_el*
insert_element_into_tree(
    struct avl_el* el,
    struct avl_el** root
) {
    if (!root || !el) {
        return NULL;
    }
    if (*root == NULL) {
        *root = el;
        return *root;
    }

    if (el->hash < (*root)->hash) {
        (*root)->l = insert_element_into_tree(el, &(*root)->l);
    } else {
        (*root)->r = insert_element_into_tree(el, &(*root)->r);
    }

    regen_metadata(*root);
    return *root;
}

static void
destroy_subtree(
    struct avl_el* node //!< A node to destroy
) {
    if (node->l) {
        destroy_subtree(node->l);
    }
    if (node->r) {
        destroy_subtree(node->r);
    }

    free(node);
}

static struct avl_el*
rebalance_subtree(
    struct avl_el* root
) {
    // check whether the root node is NULL
    if (!root) {
        return NULL;
    }

    // check whether the subtrees is already balanced (see paper)
    if (avl_node_cnt(root) >
        (unsigned int) (1 << (avl_height(root) - 1) ) - 1) {
        return root;
    }

    // perform a left-rotation if there are too many nodes in the right subtree
    while (avl_node_cnt(root->r) > (2 * avl_node_cnt(root->l) + 1)) {
        // perform right-rotations in the right subtree if necessary
        while (avl_node_cnt(root->r->r) <= avl_node_cnt(root->l)) {
            root->r = rotate_right(root->r);
        }

        root = rotate_left(root);
    }

    // perform a right-rotation if there are too many nodes in the right subtree
    while (avl_node_cnt(root->l) > (2 * avl_node_cnt(root->r) + 1)) {
        // perform left-rotations in the right subtree if necessary
        while (avl_node_cnt(root->l->l) <= avl_node_cnt(root->r)) {
            root->l = rotate_left(root->l);
        }

        root = rotate_right(root);
    }

    // now rebalance the children
    root->l = rebalance_subtree(root->l);
    root->r = rebalance_subtree(root->r);

    return root;
}

static struct avl_el*
rotate_left(
    struct avl_el* node
) {
    if (!node || !node->r) {
        return NULL;
    }

    struct avl_el* new_root = node->r;

    // relocate the middle subtree
    node->r = new_root->l;

    // old root node is now child of new root node
    new_root->l = node;

    // regenerate the node's metadata
    regen_metadata(node);
    regen_metadata(new_root);

    // return new root node
    return new_root;
}

static struct avl_el*
rotate_right(
    struct avl_el* node
) {
    if (!node || !node->l) {
        return NULL;
    }

    struct avl_el* new_root = node->l;

    // relocate the middle subtree
    node->l = new_root->r;

    // old root node is now child of new root node
    new_root->r = node;

    // regenerate the node's metadata
    regen_metadata(node);
    regen_metadata(new_root);

    // return new root node
    return new_root;
}

static void
regen_metadata(
    struct avl_el* node //!< The node to regenerate
) {
    // regenerate the height
    node->height = 1 + MAX(avl_height(node->l), avl_height(node->r));

    // regenerate the node count
    node->node_cnt = 1 + avl_node_cnt(node->l) + avl_node_cnt(node->r);

    // regenerate bloom filter
    node->filter = bloom_from_hash(node->hash) |
                   node->l->filter | node->r->filter;
}
