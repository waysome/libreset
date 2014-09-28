#include "avl/avl.h"

static int
select_from_subtree(
    struct avl_el* root,
    r_predf pred,
    void* pred_etc,
    r_procf procf,
    void* dest
) {
    if (!root) {
        return 0;
    }

    int retval;

    retval = select_from_subtree(root->l, pred, pred_etc, procf, dest);
    if (retval < 0) {
        return retval;
    }

    retval = select_from_subtree(root->r, pred, pred_etc, procf, dest);
    if (retval < 0) {
        return retval;
    }

    return ll_select(&root->ll, pred, pred_etc, procf, dest);
}

int
avl_select(
    struct avl const* src,
    r_predf pred,
    void* pred_etc,
    r_procf procf,
    void* dest
) {
    return select_from_subtree(src->root, pred, pred_etc, procf, dest);
}

