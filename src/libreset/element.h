#ifndef __ELEMENT_H__
#define __ELEMENT_H__

/**
 * @file element.h
 *
 * This file contains the type definition and helper function definition for the
 * element type.
 *
 * @note For internal use only.
 */

#include "libreset/hash.h"

/**
 * Element type
 *
 * The element type contains of two basic members:
 *  - The hash of the element
 *  - A pointer to the actual element
 */
struct rs_element
{
    rs_hash hash; //!< hash of the element
    void*   data; //!< ptr to the actual element
};

/**
 * Initialize a rs_element object
 */
static inline void
rs_element_init(
        struct rs_element* el, //!< The element to initialize
        void* data, //!< The data for the element
        struct rs_hash (*hashf)(void*) //!< The hash functions
);

/**
 * Compare two elements (compare by hash)
 *
 * @return -1 if `a` is bigger, zero if equal, 1 if `b` is bigger
 */
static inline signed int
rs_element_cmp(
        struct rs_element* a, //!< First element
        struct rs_element* b  //!< Second element
);

/**
 * Check if two elements have the same hash
 *
 * @return 1 if equal (by hash compare), zero if not
 */
static inline int
rs_element_eq(
        struct rs_element* a, //!< First element
        struct rs_element* b  //!< Second element
);

/*
 *
 * inline implementations
 *
 */

static inline void
    __fastcall__
    __pure__
rs_element_init(
        struct rs_element* el,
        void* data,
        struct rs_hash (*hashf)(void*)
) {
    el->data = data;
    el->hash = hashf(data);
}

static inline signed int
    __fastcall__
    __hot__
    __pure__
rs_element_cmp(
        struct rs_element* a,
        struct rs_element* b
) {
    return rs_hash_cmp(a->hash, b->hash);
}

static inline int
    __fastcall__
    __hot__
    __pure__
rs_element_eq(
        struct rs_element* a,
        struct rs_element* b
) {
    return rs_hash_eq(a->hash, b->hash);
}

#endif
