#include "avl.h"


/**
 * Checks if node_a is a subset of node_b
 *
 * @return 1 if node_a is a subset, else 0
 */
static int
node_is_subset(
    struct avl_el const* node_a,
    struct avl_el const* node_b,
    struct avl_el const* node_b_root,
    struct r_set_cfg const* cfg
) {
    if (!node_a) {
        // An empty set is always a subset of any other set
        return 1;
    }

    if (!node_b) {
        // However, no empty is a superset of another (non-empty set)
        return 0;
    }

    // Recursively go down the tree while taking care of the order of the hashes
    if (node_a->hash > node_b->hash) {
        return node_is_subset(node_a->l, node_b, node_b_root, cfg) &&
            node_is_subset(node_a, node_b->r, node_b_root, cfg);
    } else if (node_a->hash < node_b->hash) {
        return node_is_subset(node_a->r, node_b, node_b_root, cfg) &&
            node_is_subset(node_a, node_b->l, node_b_root, cfg);
    }

    // Verify that the node is in the tree by going back to the roots
    return node_is_subset(node_a->l, node_b_root, node_b_root, cfg) &&
            node_is_subset(node_a->r, node_b_root, node_b_root, cfg) &&
            ll_is_subset(&node_a->ll, &node_b->ll, cfg);
}


int
avl_is_subset(
    struct avl const* avl_a,
    struct avl const* avl_b,
    struct r_set_cfg const* cfg
) {
    if (!avl_a) {
        // An empty set is always a subset of another
        return 1;
    }

    return node_is_subset(avl_a->root, avl_b->root, avl_b->root, cfg);
}
