/**
 * @file avl.h
 *
 * This is the interface definition for the AVL tree implementation. The
 * implementation of the AVL element type is public, but the provided functions
 * should be used when interacting with it.
 *
 * @authors Matthias Beyer
 *
 * @copyright See the LICENSE file shipped with the repository.
 */

/*
 * @addtogroup internal_avl_interface "(internal) AVL interface definition"
 *
 * This group contains the definition of the interface for the AVL tree
 * implementation, meaning public (but only for internal use) functions.
 *
 * @{
 */

#ifndef __AVL_H__
#define __AVL_H__

#include "bloom.h"
#include "ll/ll.h"
#include "util/attributes.h"
#include "libreset/hash.h"
#include "libreset/set.h"

/**
 * AVL Tree type
 */
struct avl {
    struct avl_el* root;
};

/**
 * AVL Tree element data type
 *
 * This data type defines an AVL element. All operations for the AVL tree
 * implementation operate on this data type. There is no special root node type.
 */
struct avl_el {
    struct ll ll;           //!< The linked list containing stored elements
    r_hash hash;
    bloom filter;           //!< Bloom filter associated with the subtree
    unsigned int height;    //!< The height of the subtree
    unsigned int node_cnt;  //!< The number of nodes which are in the subtree
    struct  avl_el* l;      //!< Next left node
    struct  avl_el* r;      //!< Next right node
};

/**
 * Destroy an avl tree
 *
 * @memberof avl
 *
 * @return 0 on success, else a negative error number:
 *         -EEXIST - if the avl doesn't exist (NULL passed, no root node)
 */
int
avl_destroy(
    struct avl* avl, //!< The avl tree
    struct r_set_cfg const* cfg //!< type information proveded by the user
)
;

/**
 * Add an element to an avl tree
 *
 * @memberof avl
 *
 * @return 0 on success, else negative error number (errno.h)
 *         -ENOMEM - on allocation failed
 *         -EEXIST - if the element is already in the set
 */
int
avl_insert(
    struct avl* avl, //!< The avl tree where to insert
    r_hash hash, //!< hash value associated with d
    void* const d, //!< The data element
    struct r_set_cfg const* cfg //!< type information provided by the user
)
;

/**
 * Deletes one element from the avl tree which has an equivalent hash
 *
 * This function will remove one element from the AVL with the hash `hash` and
 * satisfying a predicate defined by the user through the configuration `cfg`.
 *
 * @memberof avl
 *
 * @return 0 if the element was removed, errno const otherwise:
 *         -EEXIST - if the element was not found in the avl
 */
int
avl_del(
    struct avl* avl, //!< The avl where to search in
    r_hash hash, //!< hash value
    void const* cmp, //!< element to compare against
    struct r_set_cfg const* cfg //!< type information provided by the user
)
;

/**
 * Delete elements from the avl tree by predicate
 *
 * This function will remove elements from the avl tree which match the
 * predicate defined in the configuration `cfg`.
 *
 * @memberof avl
 *
 * @reutrn the number of removed elements
 */
unsigned int
avl_ndel(
    struct avl* el, //!< The avl where to search in
    r_predf pred, //!< A predicate selecting what to remove
    void* etc, //!< User-data to pass to the predicate
    struct r_set_cfg const* cfg //!< type information provided by the user
)
;

/**
 * Find an element by hash `hash` satisfying the compare function in `cfg`
 *
 * @memberof avl
 *
 * @return The found data or NULL if there was nothing found
 */
void*
avl_find(
    struct avl const* avl, //!< The avl where to search in
    r_hash hash, //!< The hash value associated with d
    void const* const d, //!< The data element to compare to
    struct r_set_cfg const* cfg //!< type information proveded by the user
)
__r_warn_unused_result__
;

/**
 * Get the cardinality of this avl tree (number of elements, not nodes)
 *
 * A cardinality of an object in mathematics is, simply put, the count of
 * objects in it. It is usually denoted |Object|.
 * @memberof avl
 *
 * @return The cardinality
 */
size_t
avl_cardinality(
    struct avl const* avl //!< The avl where to count
)
__r_nonnull__(1)
__r_warn_unused_result__
;

/**
 * Determine the union of two sets (represented by AVLs) by merging
 *
 * This method merges all the elements stored in one AVL into another by
 * copying them over, if they did not yet exist.
 * Elements which are already in the destination AVL will not be copied.
 * This is equivalent to determining the union of both AVLs and storing the
 * result in the destination.
 * The function will not modify the source AVL.
 *
 * @note This function may also be used to copy an AVL into another
 *
 * @memberof avl
 *
 * @returns 0 on success, a negative error value (errno.h) on error:
 *          -ENOMEM - an allocation failed
 */
int
avl_union(
    struct avl* dest, //!< destination of the union merge
    struct avl const* src, //!< AVL to merge into the destination
    struct r_set_cfg const* cfg //!< type information proveded by the user
)
__r_nonnull__(1, 2)
;

/**
 * Get the hash value of an element
 *
 * @memberof avl_el
 *
 * @return The elements hash value.
 */
static inline r_hash
avl_get_hash(
    struct avl_el const* el //!< The element to get the hash for
)
__r_warn_unused_result__
;

/**
 * Get the height of the avl sub tree
 *
 * @memberof avl_el
 *
 * @return The height of the avl sub tree
 */
inline static unsigned int
avl_height(
    struct avl_el const* root //!< The root element
)
__r_warn_unused_result__
;


/**
 * Get the number of nodes a subtree contains
 *
 * @memberof avl_el
 *
 * @return The number of nodes the subtree contains
 */
inline static unsigned int
avl_node_cnt(
    struct avl_el const* root //!< The root element
)
__r_warn_unused_result__
;


/*
 *
 *
 * inline implementations
 *
 *
 */

static inline r_hash
avl_get_hash(struct avl_el const* el) {
    return el->hash;
}

inline static unsigned int
avl_height(
    struct avl_el const* root //!< The root element
) {
    if (root == NULL) {
        return 0;
    }
    return root->height;
}

inline static unsigned int
avl_node_cnt(
    struct avl_el const* root //!< The root element
) {
    if (!root) {
        return 0;
    }
    return root->node_cnt;
}

/** @} */

#endif //__AVL_H__
