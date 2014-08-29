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
    rs_hash hash;
    void*   data;       //!< pointer to the data of the node
    signed int height;  //!< The height of the subtree
    struct  avl_el* l;  //!< Next left node
    struct  avl_el* r;  //!< Next right node
};

/**
 * Allocate a new avl tree
 *
 * @note Alias for avl_add(NULL, NULL);
 *
 * @return A pointer to a new avl_el object or NULL on failure
 */
struct avl*
avl_alloc(void);

/**
 * Destroy an avl tree
 *
 * @return 1 on success, else a negative error number
 */
int
avl_destroy(
    struct avl* avl //!< The avl tree
);

/**
 * Add an entry in an avl tree
 *
 * @return The new added element or NULL on failure
 */
struct avl_el*
avl_add(
    struct avl* avl, //!< The avl tree where to insert
    void* const d, //!< The data element
);

/**
 * Delete an element from the avl tree
 *
 * The element of the delete operation is returned, so we do not have a dangling
 * pointer laying around.
 *
 * @return the element on success, else NULL
 */
struct avl_el*
avl_del(
    struct avl* avl, //!< The avl where to search in
    int (*pred)(void* const, void*), //!< the predicate function
    void* etc //!< an additional parameter to the predicate function
);

/**
 * Find an element in a avl tree
 *
 * @return The found element or NULL if there was nothing found
 */
struct avl_el*
avl_find(
    struct avl* avl , //!< The avl where to search in
    int (*pred)(void* const, void*), //!< the predicate function
    void* etc //!< an additional parameter to the predicate function
);

/**
 * Unlink a subtree from a tree
 *
 * @return The new root element or NULL on failure
 */
struct avl*
avl_unlink(
    struct avl* avl, //!< The root of the tree
    int (*pred)(void* const, void*), //!< the predicate function
    void* etc //!< an additional parameter to the predicate function
);

/**
 * Get the hash value of an element
 *
 * @return The elements hash value.
 */
static inline struct rs_hash
avl_get_hash(
    struct avl_el* el //!< The element to get the hash for
);

/**
 * Get the height of the avl sub tree
 *
 * @return The height of the avl sub tree
 */
inline static signed int
avl_height(
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
__fastcall__
__pure__
avl_get_hash(struct avl_el* el) {
    return el->hash;
}

inline static signed int
avl_height(
    struct avl_el* root //!< The root element
) {
    return root == NULL ? 0 : root->height;
}

/** @} */

#endif //__AVL_H__
