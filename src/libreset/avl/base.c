#include <stdlib.h>

#include "libreset/hash.h"

#include "util/likely.h"
#include "util/macros.h"
#include "util/debug.h"

#include "ll/ll.h"
#include "avl.h"
#include "common.h"
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

/**
 * Remove an element
 *
 * @return 1 if an element was removed, 0 otherwise
 */
static int
remove_element(
    struct avl_el** root, //!< The avl where to search in
    rs_hash hash, //!< hash value associated with d
    void const* cmp, //!< element to compare against
    struct r_set_cfg const* cfg //!< type information provided by the user
);

/**
 * Insert an element into the subtree denoted by it's root
 *
 * @return 1 if the insertion was successful, 0 otherwise
 */
static int
insert_element_into_tree(
    void* el, //!< The element to insert
    rs_hash hash, //!< hash of the element to insert
    struct avl_el** root, //!< The root element of the tree where to insert
    struct r_set_cfg const* cfg //!< type information proveded by the user
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
    struct r_set_cfg const* cfg
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
    struct r_set_cfg const* cfg
) {
    if (avl && avl->root) {
        destroy_subtree(avl->root, cfg);
        avl->root = NULL;
    }
    return 1;
}

void*
avl_find(
    struct avl const* avl,
    rs_hash hash,
    void const* const d,
    struct r_set_cfg const* cfg
) {
    avl_dbg("Finding element for hash: 0x%x", hash);
    struct avl_el* node = find_node(avl, hash);

    if(!node) {
        return NULL;
    }

    return ll_find(&node->ll, d, cfg);
}

int
avl_insert(
    struct avl* avl, //!< The avl tree where to insert
    rs_hash hash,
    void* const d, //!< The data element
    struct r_set_cfg const* cfg
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
    void const* cmp,
    struct r_set_cfg const* cfg
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
    struct r_set_cfg const* cfg
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

static int
insert_element_into_tree(
    void* d,
    rs_hash hash,
    struct avl_el** root,
    struct r_set_cfg const* cfg
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

static int
remove_element(
    struct avl_el** root,
    rs_hash hash,
    void const* cmp,
    struct r_set_cfg const* cfg
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

static unsigned int
delete_elements_by_predicate(
    struct avl_el** root,
    r_predf pred,
    void* etc,
    struct r_set_cfg const* cfg
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

