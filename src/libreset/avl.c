#include <stdlib.h>

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
 *
 * This function regenerates the buffered metadata of a node with a depth of 1.
 * The function does not recurse but only takes into account the metadata of
 * direct children.
 */
static void
regen_metadata(
    struct avl_el* node //!< The node to regenerate
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
    free(avl);
    return 1;
}


/*
 *
 *
 * implementation of internal functions
 *
 *
 */

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
rotate_left(
    struct avl_el* node
) {
    if (!node || !node->r) {
        return NULL;
    }

    struct avl_el* new_root=node->r;

    // relocate the middle subtree
    node->r=new_root->l;

    // old root node is now child of new root node
    new_root->l=node;

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

    struct avl_el* new_root=node->l;

    // relocate the middle subtree
    node->l=new_root->r;

    // old root node is now child of new root node
    new_root->r=node;

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
}

