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

/**
 * @addtogroup internal_linkedlist_interface
 *
 * This group contains the interface definition for the linked list type used in
 * the codebase.
 *
 * @{
 */

#ifndef __LL_H__
#define __LL_H__

#include <stddef.h>

/**
 * Linked list element type
 *
 * The linked list is a single linked one, so we only hold a pointer to the next
 * element.
 */
struct ll_element {
    struct ll_element*  next; /**< Pointer to next element of linked list */
    void*               data; /**< Pointer to the data in this node */
};

/**
 * Linked list type
 *
 * This type is used as interface to the world.
 * It was introducted for hiding the head pointer of a linked list, so we can
 * modify this one with more ease.
 */
struct ll {
    struct ll_element* head; /**< Head of the linked list */
};

/**
 * Destroy a linked list object
 *
 * @memberof ll
 *
 * Removes the linked list from the memory but does not remove the referenced
 * elements.
 */
void
ll_destroy(
    struct ll* ll /**< Ptr to the struct ll object */
);

/**
 * Insert a struct ll_element into a struct ll
 *
 * @memberof ll
 *
 * @return the struct ll object which was passed or NULL on failure
 */
struct ll*
ll_insert(
    struct ll* ll, /**< Ptr to the linked list object */
    struct ll_element* e /**< Ptr to the element to insert */
);

/**
 * Insert a element from the actual void* which references the data
 *
 * @memberof ll
 *
 * @return the struct ll object which was passed or NULL on failure
 */
struct ll*
ll_insert_data(
    struct ll* ll, /**< Ptr to the linked list object */
    void* data /**< Ptr to the data to insert */
);

/**
 * Delete a struct ll_element from the linked list
 *
 * @memberof ll
 *
 * @return the struct ll object which was passed or NULL on failure
 */
struct ll*
ll_delete(
    struct ll* ll, /**< Ptr to the linked list object */
    struct ll_element* del /**< Ptr to element to delete */
);

/**
 * Loop through an linked list
 *
 * Helper macro for looping through an linked list.
 * The first parameter should be the name of the iterator,
 * the second parameter is the linked list to iterate through.
 */
#define ll_foreach(it,ll) \
    for (struct ll_element* it = (ll)->head; it; it = it->next)

/**
 * Allocate an struct ll_element object
 *
 * @memberof ll_element
 * @static
 *
 * @return Ptr to the new struct ll_element object or NULL on failure
 */
struct ll_element*
ll_element_alloc(void);

/**
 * Allocate an struct ll_element object with data
 *
 * @memberof ll_element
 *
 * @return Ptr to the new struct ll_element object or NULL on failure
 */
struct ll_element*
ll_element_alloc_new(
    void* data /**< The data ptr for the new object */
);

/**
 * Destroy an struct ll_element object
 *
 * @memberof ll_element
 *
 * @warning The ptr to the next element will be lost, if not saved beforehand
 * @warning The ptr to the data of the element will be lost, if not saved
 * beforehand.
 */
void
ll_element_destroy(
    struct ll_element* /**< Ptr to the element to destroy */
);

/*
 * Check if the `struct ll` object is empty
 */
static int
ll_is_empty(
    struct ll* //!< The linked list object to test for emptyness
);

/*
 *
 * static inline implementations
 *
 */

static inline int
ll_is_empty(
    struct ll* ll
) {
    return (ll == NULL || ll->head == NULL);
}

/**
 * @}
 */

#endif
