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

