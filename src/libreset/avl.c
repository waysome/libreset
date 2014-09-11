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

