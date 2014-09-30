#include <stdlib.h>

#include "libreset/hash.h"

#include "util/likely.h"
#include "util/macros.h"
#include "util/debug.h"

#include "ll/ll.h"
#include "avl/avl.h"
#include "avl/common.h"
#include "util/macros.h"

struct avl_el*
new_avl_el(
    r_hash h
) {
    struct avl_el* el = calloc(1, sizeof(*el));
    if (el) {
        el->hash = h;
    }
    return el;
}

void
destroy_subtree(
    struct avl_el* node, //!< A node to destroy
    struct r_set_cfg const* cfg
) {
    avl_dbg("Destroying subtree from node %p", (void*) node);

    if (!node) {
        return;
    }

    if (node->l) {
        destroy_subtree(node->l, cfg);
    }
    if (node->r) {
        destroy_subtree(node->r, cfg);
    }

    ll_destroy(&node->ll, cfg);
    free(node);
}

struct avl_el*
rebalance_subtree(
    struct avl_el* root
) {
    avl_dbg("Rebalance subtree for %p", (void*) root);
    // check whether the root node is NULL
    if (!root) {
        return NULL;
    }

    // check whether the subtrees is already balanced (see paper)
    if (avl_node_cnt(root) >
            (unsigned int) (1 << (avl_height(root) - 1) ) - 1) {
        avl_dbg("Subtree already balanced for %p", (void*) root);
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

struct avl_el*
rotate_left(
    struct avl_el* node
) {
    avl_dbg("Rotate left around %p", (void*) node);
    if (!node || !node->r) {
        return node;
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

struct avl_el*
rotate_right(
    struct avl_el* node
) {
    avl_dbg("Rotate right around %p", (void*) node);
    if (!node || !node->l) {
        return node;
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

struct avl_el*
isolate_root_node(
    struct avl_el* node
) {
    avl_dbg("Isolate node from tree: %p", (void*) node);

    // if the node has no left child, we may use the right one as new root node
    if (!node->l) {
        return node->r;
    }

    // assume that a suitable replacement exists in the right subtree
    struct avl_el* new_root = isolate_leftmost(&node->r);

    // seems like a replacement does not exist. The right subtree must be empty
    if (!new_root) {
        return node->l;
    }

    // insert the new node
    new_root->l = node->l;
    new_root->r = node->r;
    regen_metadata(new_root);

    return new_root;
}

struct avl_el*
isolate_leftmost(
    struct avl_el** root
) {
    if (!root || !*root) {
        return NULL;
    }
    avl_dbg("Isolate leftmost node for tree %p", (void*) *root);

    // recurse
    struct avl_el* retval = isolate_leftmost(&(*root)->l);

    // if the recursion solved the problem already, we can return
    if (retval) {
        regen_metadata(*root);
        return retval;
    }

    // else the lowest element is the one at hand
    retval = *root;

    // all that is left to do is cutting the element loose
    *root = retval->r;
    return retval;
}

void
regen_metadata(
    struct avl_el* node //!< The node to regenerate
) {
    avl_dbg("Regenerate metadata for node %p", (void*) node);

    // regenerate the height
    node->height = 1 + MAX(avl_height(node->l), avl_height(node->r));

    // regenerate the node count
    node->node_cnt = 1 + avl_node_cnt(node->l) + avl_node_cnt(node->r);

    // regenerate bloom filter
    node->filter = bloom_from_hash(node->hash);
    if (node->l) {
        node->filter |= node->l->filter;
    }
    if (node->r) {
        node->filter |= node->r->filter;
    }
}

struct avl_el*
find_node(
    struct avl const* avl,
    r_hash hash
) {
    avl_dbg("Finding node with hash: 0x%zx", hash);

    struct avl_el* iter = avl->root;
    bloom filter = bloom_from_hash(hash);

    while (iter && iter->hash != hash) {
        //check whether the element _can_ be in the subtree
        if (!bloom_may_contain(filter, iter->filter)) {
            return NULL;
        }

        if (iter->hash > hash) {
            iter = iter->l;
        } else {
            iter = iter->r;
        }
    }

    return iter;
}

struct avl_el*
find_closest_lower(
    struct avl_el* root,
    r_hash hash
) {
    avl_dbg("Finding node closest to but lower or equal: 0x%zx", hash);

    // clear the domain of greater keys
    while (root) {
        if (root->hash < hash) {
            break;
        }
        root = root->l;
    }

    // if we are on a node, we are on a node with a lower key/hash
    struct avl_el* retval = root;

    // walk to the right until we hit an element with a greater hash
    while (root) {
        if (root->hash > hash) {
            return retval;
        }
        retval = root;
        root = root->r;
    }

    // we hit the bottom of the tree
    return retval;
}

struct avl_el*
find_closest_greater(
    struct avl_el* root,
    r_hash hash
) {
    avl_dbg("Finding node closest to but greater or equal: 0x%zx", hash);

    // clear the domain of lower keys
    while (root) {
        if (root->hash > hash) {
            break;
        }
        root = root->r;
    }

    // if we are on a node, we are on a node with a greater key/hash
    struct avl_el* retval = root;

    // walk to the right until we hit an element with a lower hash
    while (root) {
        if (root->hash < hash) {
            return retval;
        }
        retval = root;
        root = root->l;
    }

    // we hit the bottom of the tree
    return retval;
}

