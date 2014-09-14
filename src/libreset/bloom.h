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
 * @addtogroup internal_bloom_filter_interface
 *
 * This group contains the interface definition for the bloom filter type used
 * in the codebase.
 *
 * @{
 */

#ifndef __BLOOM_H__
#define __BLOOM_H__

#include <stddef.h>

#include "libreset/hash.h"

#include "util/attributes.h"

/**
 * Bloom filter type
 *
 * The bloom filter is a probabilistic set representation used for speedups.
 */
typedef size_t bloom;

/**
 * Get the bloom filter for an element with a given hash.
 *
 * @memberof bloom
 * @return bloom filter for the given hash
 */
__const__ bloom
bloom_from_hash(
    rs_hash hash //!< hash value for which to calculate the bloom filter
);

/**
 * Check whether an element may be in a bloom filter
 *
 * This is much faster than checking whether two sets may have common elements
 *
 * @memberof bloom
 * @return 0 if the element is not in the set, 1 otherwise
 */
__const__ int
bloom_may_contain(
    bloom element, //!< element, represented as a bloom filter
    bloom set //!< bloom filter representing the set to check
);

/**
 * Check whether two bloom filters may contain common elements
 *
 * @memberof bloom
 * @return 0 if the sets represented are exclusively disjunctive, 1 otherwise
 */
__const__ int
bloom_may_have_common(
    bloom a, //!< first of the bloom filters to check for common elements
    bloom b //!< second of the bloom filters to check for common elements
);

/**
 * @}
 */

#endif
