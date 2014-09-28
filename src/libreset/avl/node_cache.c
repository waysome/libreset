#include "avl/node_cache.h"

int
avl_node_is_in_cache(
    struct avl_node_cache const* cache,
    struct avl_el const* node
) {
    while (cache) {
        if (cache->node == node) {
            return 1;
        }
        cache = cache->next;
    }
    return 0;
}


