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
    struct avl_el* node //!< A node to destroy
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

/*
 * Swap two avl elements by swapping their values
 *
 * @warning This is kind of a hack. The linkedlist object inside the avl_el
 * object is accessed directly. This is neither neat nor nice, but it works good
 * and fast.
 */
static inline void
swp_avl_els(
    struct avl_el* a, //!< First element
    struct avl_el* ap, //!< First elements parent node
    struct avl_el* b, //!< Second element
    struct avl_el* bp  //!< Second elements parent node
);

/**
 * Delete a node from a subtree
 *
 * Although the node to delete and its parent are already known, this function
 * uses the recursive approach to re-find them. This is done because we are then
 * easily able to call the rebalance function when coming up from the recursion
 * steps.
 */
static void
delete_node_from_subtree(
    struct avl_el* node, //!< The node to delete
    struct avl_el* parent, //!< The parent to delete
    struct avl_el* root //!< The root node of the subtree
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
    return 1;
}

struct avl_el*
avl_find(
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

struct avl_el*
avl_add(
    struct avl* avl, //!< The avl tree where to insert
    void* const d, //!< The data element
    rs_hash hash //!< The hash for the data element
) {
    struct avl_el* element = avl_find(avl, hash);

    if (element) {
        ll_insert_data(&element->ll, d);
    } else {
        element = new_avl_el(hash);
        if (element) {
            ll_insert_data(&element->ll, d);
            insert_element_into_tree(element, &avl->root);
            rebalance_subtree(avl->root);
        } else {
            /* Out of memory */
        }
    }

    return element;
}

struct avl_el*
avl_del(
    struct avl* avl,
    rs_hash hash,
    rs_predicate_function pred,
    void* etc
) {
    if (!avl || !avl->root) {
        return NULL;
    }

    struct avl_el* parent = NULL;
    struct avl_el* found = avl->root;

    while (found->hash != hash) {
        parent = found;
        if (found->hash > hash) {
            found = found->l;
        } else {
            found = found->r;
        }
    }

    if (found == avl->root && found->hash != hash) {
        /* We haven't found an element with the hash */
        return NULL;
    }

    /* We have found the element */
    struct ll_element* ll_del_element;
    ll_foreach(lliter, &(found->ll)) {
        if (pred(lliter->data, etc)) {
            ll_del_element = lliter;
            break;
        }
    }
    ll_delete(&(found->ll), ll_del_element);

    if (!ll_is_empty(&(found->ll))) {
        /* If there are still elements in the avl element, we can return it
         * because we are ready now */
        return found;
    }

    delete_node_from_subtree(found, parent, avl->root);

    rebalance_subtree(avl->root);

    return avl->root;
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

static inline void
swp_avl_els(
    struct avl_el* a,
    struct avl_el* ap,
    struct avl_el* b,
    struct avl_el* bp
) {
    struct avl_el** ap_c = (ap->l == a) ? &ap->l : &ap->r;
    struct avl_el** bp_c = (bp->l == b) ? &bp->l : &bp->r;

    struct avl_el* tmp_lptr = a->l;
    struct avl_el* tmp_rptr = a->r;

    a->l = b->l;
    a->r = b->r;

    b->l = tmp_lptr;
    b->r = tmp_rptr;

    *ap_c = b;
    *bp_c = a;

    /**
     * @todo Do we need to swap the meta attributes of a node here, too?
     */
}

static void
delete_node_from_subtree(
    struct avl_el* node,
    struct avl_el* parent,
    struct avl_el* root
) {
    if (node->hash != root->hash) {
        if (node->hash < root->hash) {
            delete_node_from_subtree(node, parent, root->l);
        } else {
            delete_node_from_subtree(node, parent, root->r);
        }
        rebalance_subtree(root);
    } else {

        /*
         * We must delete the whole avl element and rebalance the tree
         *
         * Three cases are possible now:
         * 1) The found element has no children. We can simply remove it then
         *    (and fix the appropriate pointer in the parent).
         *
         * 2) The found element has one child. So we delete the found element
         *    and let the parent point to the child of found.
         *
         * 3) The found element has two children.
         *
         *      3.1) Find the successor of `found` (`z`), which should be the
         *           leftmost node in the right subtree of `found`.
         *      3.2) Replace `found` with `z`
         *      3.3) Delete `z`
         *           Note: `z` does not have a left child
         *           Note: `z` has at most one child, we can use case (1) or (2)
         *                 to delete `z`
         */

        if (!node->l && !node->r) {
            /* case 1 */
            if (parent->l == node) {
                parent->l = NULL;
            } else {
                parent->r = NULL;
            }
            free(node);
        } else if ((node->l && !node->r) || (!node->l && node->r)) {
            /* case 2 */
            struct avl_el* link = (node->l) ? node->l : node->r;

            if (parent->l == node) {
                parent->l = link;
            } else {
                parent->r = link;
            }
            free(node);
        } else {
            /* case 3 */

            /* find successor of `node`, call it `z`, its parent `zp` */
            struct avl_el* zp = node;
            struct avl_el* z = node->r;
            while (z->l) {
                zp = z;
                z = z->l;
            }

            /*
             * Possible child of `z`, lets call it `z_ch`.
             * It cannot be the left.
             */
            struct avl_el* z_ch = z->r;

            /* replace `z` with `node` */
            swp_avl_els(z, zp, node, parent);

            /*
             * If `z` had a child, set the ptr of the parent to it.
             * The left ptr of the parent must be set, as `z` was the left child
             * of its parent.
             */
            if (z_ch) {
                zp->l = z_ch;
            }

            free(z);
        }

    }
}

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
    node->filter = bloom_from_hash(node->hash) |
                   node->l->filter | node->r->filter;
}
