#include "libreset/hash.h"

#include "avl.h"

/**
 * Debug print helper for avl implementation code
 *
 * @note No #ifdef DEBUG here, because if dbg() evaluates to nothing, this code
 * gets removed by the compiler anyways.
 */
#define avl_dbg(fmt,...) do { dbg("avl: "fmt, __VA_ARGS__); } while (0)


void
destroy_subtree(
    struct avl_el* node, //!< A node to destroy
    struct r_set_cfg const* cfg //!< type information proveded by the user
);

/**
 * Rebalance a subtree
 *
 * This function will recursively rebalance a subtree.
 * It uses the metadata of a subtree's root node to check whether the subtree
 * already is balanced. This way, unnecessary recursions are omitted.
 *
 * @return new root
 */
struct avl_el*
rebalance_subtree(
    struct avl_el* root //!< The root of the subtree to rebalance
)
__r_warn_unused_result__
;

/**
 * Rotate a node counter-clockwise
 *
 * @return new root or NULL, if the rotation could not be performed
 */
struct avl_el*
rotate_left(
    struct avl_el* node //!< The node to rotate
)
__r_warn_unused_result__
;

/**
 * Rotate a node clockwise
 *
 * @return new root or NULL, if the rotation could not be performed
 */
struct avl_el*
rotate_right(
    struct avl_el* node //!< The node to rotate
)
__r_warn_unused_result__
;

/**
 * Isolate the root node of a given subtree
 *
 * @return the new root node of the subtree
 * @warning This function will crash when being passed NULL.
 */
struct avl_el*
isolate_root_node(
    struct avl_el* node //!< node to isolate
)
__r_warn_unused_result__
;

/**
 * Isolate node with lowest key from subtree
 *
 * This function will isolate the node with the lowest key from the rest of the
 * given subtree. The lowest key, in this implementation, is the leftmost node.
 * The isolated node will be returned.
 *
 * @return node with lowest key, or NULL
 */
struct avl_el*
isolate_leftmost(
    struct avl_el** root //!< Pointer to the root of the affected subtree
)
__r_warn_unused_result__
;

/**
 * Regenerate a node's height and node_cnt
 *
 * @return void
 * @warning This function will crash when being passed NULL.
 *
 * This function regenerates the buffered metadata of a node with a depth of 1.
 * The function does not recurse but only takes into account the metadata of
 * direct children.
 */
void
regen_metadata(
    struct avl_el* node //!< The node to regenerate
);

/**
 * Create a new struct avl_el object
 *
 * @return Ptr to the new struct avl_el object or NULL on failure
 */
struct avl_el*
new_avl_el(
    r_hash h //!< The hash for the new struct avl_el object
)
__r_warn_unused_result__
;

/**
 * Find a node by it's key/hash
 *
 * @return found node or NULL, if the node does not exist
 */
struct avl_el*
find_node(
    struct avl const* avl,
    r_hash hash
)
__r_warn_unused_result__
;

