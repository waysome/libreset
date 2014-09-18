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
    struct avl_el* node, //!< A node to destroy
    struct r_set_cfg* cfg //!< type information proveded by the user
)
__attribute__((nonnull (1, 2)))
;

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
)
__attribute__((nonnull (1)))
;

/**
 * Rotate a node counter-clockwise
 *
 * @return new root or NULL, if the rotation could not be performed
 */
static struct avl_el*
rotate_left(
    struct avl_el* node //!< The node to rotate
)
__attribute__((nonnull (1)))
;

/**
 * Rotate a node clockwise
 *
 * @return new root or NULL, if the rotation could not be performed
 */
static struct avl_el*
rotate_right(
    struct avl_el* node //!< The node to rotate
)
__attribute__((nonnull (1)))
;

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

/**
 * Find a node by it's key/hash
 *
 * @return found node or NULL, if the node does not exist
 */
static struct avl_el*
find_node(
    struct avl* avl,
    rs_hash hash
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
    struct avl_el* node = find_node(avl, hash);

    // TODO: exchange with ll_find as soon as it's present
    ll_foreach(it, &node->ll) {
        if (cfg->cmpf(it->data, d)) {
            return it->data;
        }
    }

    return NULL;
}

struct avl_el*
avl_add(
    struct avl* avl, //!< The avl tree where to insert
    rs_hash hash,
    void* const d, //!< The data element
    struct r_set_cfg* cfg
) {
    struct avl_el* element = find_node(avl, hash);

    if (element) {
        ll_insert(&element->ll, d, cfg);
    } else {
        element = new_avl_el(hash);
        if (element) {
            ll_insert(&element->ll, d, cfg);
            insert_element_into_tree(element, &avl->root);
            rebalance_subtree(avl->root);
        } else {
            /* Out of memory */
        }
    }

    return element;
}

int
avl_del(
    struct avl* avl,
    rs_hash hash,
    void* cmp,
    struct r_set_cfg* cfg
) {
    int retval = remove_element(&avl->root, hash, cmp, cfg);
    avl->root = rebalance_subtree(avl->root);
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
    struct avl_el* node, //!< A node to destroy
    struct r_set_cfg* cfg
) {
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

static int
remove_element(
    struct avl_el** root,
    rs_hash hash,
    void* cmp,
    struct r_set_cfg* cfg
) {
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
    if (!(*root)->ll.head) {
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
    struct avl* avl,
    rs_hash hash
) {
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

