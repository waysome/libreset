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
#include "ll.h"
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
    rs_hash hash;
    bloom filter;           //!< Bloom filter associated with the subtree
    unsigned int height;    //!< The height of the subtree
    unsigned int node_cnt;  //!< The number of nodes which are in the subtree
    struct  avl_el* l;      //!< Next left node
    struct  avl_el* r;      //!< Next right node
};

/**
 * Allocate a new avl tree
 *
 * @note Alias for avl_add(NULL, NULL);
 *
 * @memberof avl
 * @static
 *
 * @return A pointer to a new avl_el object or NULL on failure
 */
struct avl*
avl_alloc(void);

/**
 * Destroy an avl tree
 *
 * @memberof avl
 *
 * @return 1 on success, else a negative error number
 */
int
avl_destroy(
    struct avl* avl, //!< The avl tree
    struct r_set_cfg* cfg //!< type information proveded by the user
);

/**
 * Add an entry in an avl tree
 *
 * @memberof avl
 *
 * @return The new added element or NULL on failure
 */
struct avl_el*
avl_add(
    struct avl* avl, //!< The avl tree where to insert
    rs_hash hash, //!< hash value associated with d
    void* const d, //!< The data element
    struct r_set_cfg* cfg //!< type information provided by the user
);

/**
 * Deletes one element from the avl tree which has an equivalent hash
 *
 * This function will remove one element from the AVL with the hash `hash` and
 * satisfying a predicate defined by the user through the configuration `cfg`.
 *
 * @memberof avl
 *
 * @return 1 if the element was removed, 0 otherwise
 */
int
avl_del(
    struct avl* avl, //!< The avl where to search in
    rs_hash hash, //!< hash value
    void* cmp, //!< element to compare against
    struct r_set_cfg* cfg //!< type information provided by the user
);

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
    struct r_set_cfg* cfg //!< type information provided by the user
);

/**
 * Find an element in the node which compares to the element to find
 *
 * @memberof avl
 *
 * @return The found node or NULL if there was nothing found
 */
void*
avl_find(
    struct avl* avl, //!< The avl where to search in
    rs_hash hash, //!< The hash value associated with d
    void* const d, //!< The data element to compare to
    struct r_set_cfg* cfg //!< type information proveded by the user
);

/**
 * Get the hash value of an element
 *
 * @memberof avl_el
 *
 * @return The elements hash value.
 */
static inline rs_hash
avl_get_hash(
    struct avl_el* el //!< The element to get the hash for
);

/**
 * Get the height of the avl sub tree
 *
 * @memberof avl_el
 *
 * @return The height of the avl sub tree
 */
inline static unsigned int
avl_height(
    struct avl_el* root //!< The root element
);


/**
 * Get the number of nodes a subtree contains
 *
 * @memberof avl_el
 *
 * @return The number of nodes the subtree contains
 */
inline static unsigned int
avl_node_cnt(
    struct avl_el* root //!< The root element
);


/*
 *
 *
 * inline implementations
 *
 *
 */

static inline rs_hash
avl_get_hash(struct avl_el* el) {
    return el->hash;
}

inline static unsigned int
avl_height(
    struct avl_el* root //!< The root element
) {
    return root == NULL ? 0 : root->height;
}

inline static unsigned int
avl_node_cnt(
    struct avl_el* root //!< The root element
) {
    return (root == NULL) ? 0 : (root->node_cnt);
}

/** @} */

#endif //__AVL_H__
