#include <stdlib.h>

#include "avl.h"


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
    return 0;
}

static struct avl_el*
rotate_right(
    struct avl_el* node
) {
    return 0;
}

static void
regen_metadata(
    struct avl_el* node //!< The node to regenerate
) {
    //TODO: implement
}
