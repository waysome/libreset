#include "ht/ht.h"

/**
 * Helper struct for passing information to `insert()` function
 */
struct insert_etc_data {
    struct ht* ht;
    struct r_set_cfg const* cfg;
};

/**
 * Helper function to pass around, defining how to insert something into the new
 * set
 *
 * This whole approach is based on this function. This function is passed
 * around. It is used to insert values "somewhere".
 *
 * The idea of the select-functions are: You pass a destination set and a source
 * set. You select _some_ elements from the source set (by predicate) and insert
 * them into the destination set.
 *
 * The insertion is done by this function. The linked list actually calls thif
 * function, if the predicate becomes true for one element. An alternative
 * approach _would_ be to pass the actual `struct r_set*` around, which is kind
 * of ugly.
 *
 * So, this function is passed around. It gets the element to insert (which is a
 * `void*`) and an "etc" value. The "etc" value is used to pass the destination
 * hashtable to it.
 *
 * Without this function, the linked list implementation would need to call
 * `ht_insert()` on a hashtable object it gets.
 *
 * @return 1 on failure, else zero
 */
static int
insert(
    void* element_to_insert, //!< The ptr to the element to insert in the dest
    void* insert_etc //!< Additional information via `struct insert_etc_data*`
) {
    struct insert_etc_data* etc = insert_etc;
    return (0 == ht_insert(etc->ht, element_to_insert, etc->cfg));
}

int
ht_select(
    struct ht* dest,
    struct ht const* src,
    r_predf pred,
    void* etc,
    struct r_set_cfg* const cfg
) {
    size_t i;
    size_t nbucks = ht_nbuckets(src);

    struct insert_etc_data etc_data = { .ht = dest, .cfg = cfg };

    for (i = 0; i < nbucks; i++) {
        if (!avl_select(&src->buckets[i].avl, pred, etc, insert, &etc_data)) {
            return 1;
        }
    }

    return 0;
}
