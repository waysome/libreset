/*
 * libreset - Reentrent set library for fast set operations in C
 *
 * Copyright (C) 2014 Matthias Beyer
 * Copyright (C) 2014 Julian Ganz
 *
 * This file is part of libreset.
 *
 * libreset is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * libreset is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libreset. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __AVL_NODE_HASH_H__
#define __AVL_NODE_HASH_H__

/**
 * Node for a cache for AVL nodes, as a linked list
 *
 * This NULL-terminated linked list has the purpose of caching AVL nodes in
 * recursive algorithms.
 *
 * Such an recursive algorithm must receive a pointer of this type as a
 * parameter.
 * An instance of a function may cache a node by putting a struct of this
 * type on it's stack and setting the members to the avl_node_cache pointer
 * passed to the recursive function element to cache.
 * A pointer to that new struct should then be passed to recursions.
 * If, however, no new node is added to the cache, the value originally passed
 * to the function may be passed to the sub-calls.
 *
 * The original call, entering the recursion, should either pass NULL, or a
 * prepared NULL-terminated linked list of nodes to cache.
 */
struct avl_node_cache {
    struct avl_node_cache* next; //!< pointer to next element in linked list
    struct avl_el* node; //!< node in the cache
};

/**
 * Set the next and node pointer of an avl_node_cache
 *
 * @warning only for use in direct initialisation
 */
#define AVL_NODE_CACHE_NEW_ENTRY(next_, node_) { .next = next_; .node = node_ }

/**
 * Check whether a node is in the cache
 *
 * @return 1 if the node is in the cache, 0 otherwise
 */
int
avl_node_is_in_cache(
    struct avl_node_cache const* cache, //!< head pointer of linked list
    struct avl_el const* node //!< node to check
)
;

#endif

