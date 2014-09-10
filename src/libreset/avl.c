#include <stdlib.h>

#include "libreset/avl.h"

struct avl*
avl_alloc(void) {
    return calloc(1, sizeof(struct avl));
}
