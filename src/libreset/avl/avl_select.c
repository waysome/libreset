#include "avl/avl.h"

static int
select_from_subtree(
    struct avl_el* root,
    r_predf pred,
    void* etc,
    int (*insf)(void* elem, void* insetc),
    void* insetc
) {
    if (!root) {
        return 0;
    }

    int l = 0;
    int r = 0;
    int base = 0;

    if (root->l) {
        l = select_from_subtree(root->l, pred, etc, insf, insetc);
    }

    if (root->r) {
        r = select_from_subtree(root->r, pred, etc, insf, insetc);
    }

    base = ll_select(&root->ll, pred, etc, insf, insetc);

    return (base | l | r);
}

int
avl_select(
    struct avl const* src,
    r_predf pred,
    void* etc,
    int (*insf)(void* elem, void* insetc),
    void* insetc
) {
    if (src) {
        return select_from_subtree(src->root, pred, etc, insf, insetc);
    }

    return 1;
}

