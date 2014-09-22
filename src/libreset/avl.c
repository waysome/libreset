#include <stdlib.h>

#include "libreset/hash.h"

#include "util/likely.h"
#include "util/macros.h"
#include "util/debug.h"

#include "ll.h"
#include "avl.h"
#include "util/macros.h"

/**
 * Debug print helper for avl implementation code
 *
 * @note No #ifdef DEBUG here, because if dbg() evaluates to nothing, this code
 * gets removed by the compiler anyways.
 */
#define avl_dbg(fmt,...) do { dbg("avl: "fmt, __VA_ARGS__); } while (0)

/*
 *
 *
 * forward declarations
 *
 *
 */

static void
destroy_subtree(
    struct avl_el* node, //!< A node to destroy
    struct r_set_cfg* cfg //!< type information proveded by the user
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
 * Remove an element
 *
 * @return 1 if an element was removed, 0 otherwise
 */
static int
remove_element(
    struct avl_el** root, //!< The avl where to search in
    rs_hash hash, //!< hash value associated with d
    void* cmp, //!< element to compare against
    struct r_set_cfg* cfg //!< type information provided by the user
);

/**
 * Isolate the root node of a given subtree
 *
 * @return the new root node of the subtree
 * @warning This function will crash when being passed NULL.
 */
static struct avl_el*
isolate_root_node(
    struct avl_el* node //!< node to isolate
);

/**
 * Isolate node with lowest key from subtree
 *
 * This function will isolate the node with the lowest key from the rest of the
 * given subtree. The lowest key, in this implementation, is the leftmost node.
 * The isolated node will be returned.
 *
 * @return node with lowest key, or NULL
 */
static struct avl_el*
isolate_leftmost(
    struct avl_el** root //!< Pointer to the root of the affected subtree
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
 * Insert an element into the subtree denoted by it's root
 *
 * @return 1 if the insertion was successful, 0 otherwise
 */
int
insert_element_into_tree(
    void* el, //!< The element to insert
    rs_hash hash, //!< hash of the element to insert
    struct avl_el** root, //!< The root element of the tree where to insert
    struct r_set_cfg* cfg //!< type information proveded by the user
);

/**
 * Find a node by it's key/hash
 *
 * @return found node or NULL, if the node does not exist
 */
static struct avl_el*
find_node(
    struct avl const* avl,
    rs_hash hash
);

/**
 * Delete all elements in the tree under `root` for which the predicate `pred`
 * evaluates true.
 *
 * @return Number of removed elements.
 */
static unsigned int
delete_elements_by_predicate(
    struct avl_el** root,
    r_predf pred,
    void* etc,
    struct r_set_cfg* cfg
);

/*
 *
 *
 * interface implementation
 *
 *
 */

int
avl_destroy(
    struct avl* avl, //!< The avl tree
    struct r_set_cfg* cfg
) {
    if (avl && avl->root) {
        destroy_subtree(avl->root, cfg);
    }
    return 1;
}

void*
avl_find(
    struct avl* avl,
    rs_hash hash,
    void* const d,
    struct r_set_cfg* cfg
) {
    avl_dbg("Finding element for hash: 0x%x", hash);
    struct avl_el* node = find_node(avl, hash);

    // TODO: exchange with ll_find as soon as it's present
    ll_foreach(it, &node->ll) {
        if (cfg->cmpf(it->data, d)) {
            return it->data;
        }
    }

    return NULL;
}

int
avl_insert(
    struct avl* avl, //!< The avl tree where to insert
    rs_hash hash,
    void* const d, //!< The data element
    struct r_set_cfg* cfg
) {
    avl_dbg("Adding element %p with hash: 0x%x", d, hash);

    int retval = insert_element_into_tree(d, hash, &avl->root, cfg);
    avl->root = rebalance_subtree(avl->root);

    return retval;
}

int
avl_del(
    struct avl* avl,
    rs_hash hash,
    void* cmp,
    struct r_set_cfg* cfg
) {
    avl_dbg("Deleting element with hash: 0x%x", hash);
    int retval = remove_element(&avl->root, hash, cmp, cfg);
    avl->root = rebalance_subtree(avl->root);
    return retval;
}

unsigned int
avl_ndel(
    struct avl* el,
    r_predf pred,
    void* etc,
    struct r_set_cfg* cfg
) {
    unsigned int retval = delete_elements_by_predicate(&el->root, pred, etc, cfg);
    el->root = rebalance_subtree(el->root);
    return retval;
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

int
insert_element_into_tree(
    void* d,
    rs_hash hash,
    struct avl_el** root,
    struct r_set_cfg* cfg
) {
    avl_dbg("Inserting element %p with hash: 0x%x", d, hash);
    int retval;

    // we reached the bottom of the tree
    if (*root == NULL) {
        // create new node and insert
        struct avl_el* node = new_avl_el(hash);
        if (!node) {
            // out of memory
            return 0;
        }
        retval = ll_insert(&node->ll, d, cfg);

        *root = node;
        regen_metadata(*root);
        return retval;
    }

    // recurse if neccessary
    if (hash < (*root)->hash) {
        retval = insert_element_into_tree(d, hash, &(*root)->l, cfg);
        regen_metadata(*root);
        return retval;
    }
    if (hash > (*root)->hash) {
        retval = insert_element_into_tree(d, hash, &(*root)->r, cfg);
        regen_metadata(*root);
        return retval;
    }

    // insert into this element, not creating new nodes
    return ll_insert(&(*root)->ll, d, cfg);
}

static void
destroy_subtree(
    struct avl_el* node, //!< A node to destroy
    struct r_set_cfg* cfg
) {
    avl_dbg("Destroying subtree from node %p", node);
    if (node->l) {
        destroy_subtree(node->l, cfg);
    }
    if (node->r) {
        destroy_subtree(node->r, cfg);
    }

    // TODO: clear linked list
    free(node);
}

static struct avl_el*
rebalance_subtree(
    struct avl_el* root
) {
    avl_dbg("Rebalance subtree for %p", root);
    // check whether the root node is NULL
    if (!root) {
        return NULL;
    }

    // check whether the subtrees is already balanced (see paper)
    if (avl_node_cnt(root) >
        (unsigned int) (1 << (avl_height(root) - 1) ) - 1) {
        avl_dbg("Subtree already balanced for %p", root);
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
    avl_dbg("Rotate left around %p", node);
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

static struct avl_el*
rotate_right(
    struct avl_el* node
) {
    avl_dbg("Rotate right around %p", node);
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

static int
remove_element(
    struct avl_el** root,
    rs_hash hash,
    void* cmp,
    struct r_set_cfg* cfg
) {
    avl_dbg("Remove element with hash: 0x%x", hash);

    // check whether the subtree is empty
    if (!*root) {
        return 0;
    }

    int retval;

    // iterate into subnodes if neccessary
    if (hash < (*root)->hash) {
        retval = remove_element(&(*root)->l, hash, cmp, cfg);
        regen_metadata(*root);
        return retval;
    }

    if (hash > (*root)->hash) {
        retval = remove_element(&(*root)->r, hash, cmp, cfg);
        regen_metadata(*root);
        return retval;
    }

    // remove element from linked list
    retval = ll_delete(&(*root)->ll, cmp, cfg);

    // remove the node if neccessary
    if (ll_is_empty(&(*root)->ll)) {
        avl_dbg("Remove node from tree: %p", *root);
        // isolate the node
        struct avl_el* to_del = *root;
        *root = isolate_root_node(to_del);
        regen_metadata(*root);

        // delete the node
        free(to_del);
    }
    return retval;
}

static struct avl_el*
isolate_root_node(
    struct avl_el* node
) {
    avl_dbg("Isolate node from tree: %p", node);

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

static struct avl_el*
isolate_leftmost(
    struct avl_el** root
) {
    if (!root || !*root) {
        return NULL;
    }
    avl_dbg("Isolate leftmost node for tree %p", *root);

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

static void
regen_metadata(
    struct avl_el* node //!< The node to regenerate
) {
    avl_dbg("Regenerate metadata for node %p", node);

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

static struct avl_el*
find_node(
    struct avl const* avl,
    rs_hash hash
) {
    avl_dbg("Finding node with hash: 0x%x", hash);

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

static unsigned int
delete_elements_by_predicate(
    struct avl_el** root,
    r_predf pred,
    void* etc,
    struct r_set_cfg* cfg
) {
    // check whether the subtree is empty
    if (!*root) {
        return 0;
    }

    unsigned int retval = 0;

    // iterate into subnodes
    retval += delete_elements_by_predicate(&(*root)->l, pred, etc, cfg);
    retval += delete_elements_by_predicate(&(*root)->r, pred, etc, cfg);

    // remove elements from this node
    retval += ll_ndel(&(*root)->ll, pred, etc, cfg);

    // remove the node if neccessary
    if (ll_is_empty(&(*root)->ll)) {
        avl_dbg("Remove node from tree: %p", *root);
        // isolate the node
        struct avl_el* to_del = *root;
        *root = isolate_root_node(to_del);

        // delete the node
        free(to_del);
    }

    regen_metadata(*root);
    return retval;
}

