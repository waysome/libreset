#include "avl/avl.h"

#include "avl/common.h"
#include "ll/ll.h"
#include "errno.h"


/*
 *
 *
 * forward declarations
 *
 *
 */

/**
 * Determine the union of two AVL subtrees (recursively)
 *
 * @returns 0 on success, a negative error value (errno.h) on error:
 *          -ENOMEM - an allocation failed
 */
static int
merge_trees(
    struct avl_el** dest, //!< destination subtree
    struct avl_el const* src, //!< source subtree
    struct r_set_cfg const* cfg //!< type information provided by the user
)
__r_nonnull__(1, 2)
;

/*
 *
 *
 * interface implementations
 *
 *
 */

int
avl_union(
    struct avl* dest,
    struct avl const* src,
    struct r_set_cfg const* cfg
) {
    int retval = merge_trees(&dest->root, src->root, cfg);
    dest->root = rebalance_subtree(dest->root);
    return retval;
}

/*
 *
 *
 * internal implementations
 *
 *
 */

int
merge_trees(
    struct avl_el** dest,
    struct avl_el const* src,
    struct r_set_cfg const* cfg
) {
    // if the source is empty, all it's items are integrated into dest
    if (!src) {
        return 0;
    }

    int retval;

    // if the destination subtree is empty, we copy over the subtree
    if (!*dest) {
        *dest = new_avl_el(src->hash);
        if (!*dest) {
            retval = -ENOMEM;
            goto cleanup;
        }

        // the rest of the copy operation is done near the end of the function
    }
    // if there is a node for dest->hash in the src, it's in the left subtree
    else if (src->hash > (*dest)->hash) {
        // the current src node has to be merged in the right subtree
        retval = merge_trees(&(*dest)->r, src, cfg);
        if (retval < 0) {
            goto cleanup;
        }

        // search in the left source subtree
        retval = merge_trees(dest, src->l, cfg);

        goto cleanup;
    }
    // if there is a node for dest->hash in the src, it's in the right subtree
    else if (src->hash < (*dest)->hash) {
        // the current src node has to be merged in the left subtree
        retval = merge_trees(&(*dest)->l, src, cfg);
        if (retval < 0) {
            goto cleanup;
        }

        // search in the right source subtree
        retval = merge_trees(dest, src->r, cfg);

        goto cleanup;
    }

    // at this point src->hash == (*dest) -> hash

    // copy/recurse into the left subtrees
    retval = merge_trees(&(*dest)->l, src->l, cfg);
    if (retval < 0) {
        goto cleanup;
    }

    // copy/recurse into the right subtrees
    retval = merge_trees(&(*dest)->r, src->r, cfg);
    if (retval < 0) {
        goto cleanup;
    }

    // we have matching nodes and may now merge the corresponding lists
    retval = ll_union(&(*dest)->ll, &src->ll, cfg);

    // clean up
cleanup:
    regen_metadata(*dest);
    return retval;
}

