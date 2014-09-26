#include "avl/avl.h"

static size_t
subtree_cardinality(
    struct avl_el const* subtree
) {
    if (!subtree) {
        return 0;
    }

    return subtree_cardinality(subtree->l) +
           subtree_cardinality(subtree->r) +
           ll_count(&subtree->ll);
}

size_t
avl_cardinality(
    struct avl const* avl
) {
    if (!avl) {
        return 0;
    }

    return subtree_cardinality(avl->root);
}
