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
    struct r_set_cfg* cfg //!< type information proveded by the user
);

/**
 * Predicate function type
 *
 * This is the predicate function type for selecting nodes of the avl in an
 * algorithm by predicate.
 *
 * The function gets an additional parameter (void*), which can be used to pass
 * data to the function.
 */
typedef int (*rs_predicate_function)(void* const, void*);

/**
 * Delete an element from the avl tree
 *
 * The element of the delete operation is returned, so we do not have a dangling
 * pointer laying around.
 *
 * @memberof avl
 *
 * @return the element on success, else NULL
 */
struct avl_el*
avl_del(
    struct avl* avl, //!< The avl where to search in
    rs_hash hash, //!< hash value associated with d
    rs_predicate_function pred, //!< the predicate function
    void* etc, //!< an additional parameter to the predicate function
    struct r_set_cfg* cfg //!< type information proveded by the user
);

/**
 * Find a node in a avl tree which has the passed hash
 *
 * @memberof avl
 *
 * @return The found node or NULL if there was nothing found
 */
struct avl_el*
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
