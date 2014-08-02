#ifndef __AVL_H__
#define __AVL_H__

struct avl_el {
    void*   data;
    struct  avl_el* l;
    struct  avl_el* r;
};

/**
 * Allocate a new avl tree
 *
 * @note Alias for avl_add(NULL, NULL);
 *
 * @return A pointer to a new avl_el object or NULL on failure
 */
struct avl_el*
avl_alloc(void);

/**
 * Destroy an avl tree
 *
 * @return 1 on success, else a negative error number
 */
int
avl_destroy(
    struct avl_el* root //!< The root node of the tree
);

/**
 * Add an entry in an avl tree
 *
 * @return The new added element or NULL on failure
 */
struct avl_el*
avl_add(
    struct avl_el* root, //!< The root of the avl tree where to insert
    int (*cmp)(void* const, void* const), //!< the comperator function
    void* const d //!< The data element
);

/**
 * Delete an element from the avl tree
 *
 * @return 1 on success, else a negative error code
 */
int
avl_del(
    struct avl_el* root, //!< The root where to start the search from
    int (*cmp)(void* const, void* const, void*), //!< the comperator function
    void* etc //!< an additional parameter to the comperator function
);

/**
 * Find an element in a avl tree
 *
 * @return The found element or NULL if there was nothing found
 */
struct avl_el*
avl_find(
    struct avl_el* root, //!< The root where to start the search from
    int (*cmp)(void* const, void* const, void*), //!< the comperator function
    void* etc //!< an additional parameter to the comperator function
);

/**
 * Unlink a subtree from a tree
 *
 * @return The new root element or NULL on failure
 */
struct avl_el*
avl_unlink(
    struct avl_el* root, //!< The root of the tree
    int (*cmp)(void* const, void* const, void*), //!< the comperator function
    void* etc //!< an additional parameter to the comperator function
);

#endif //__AVL_H__
