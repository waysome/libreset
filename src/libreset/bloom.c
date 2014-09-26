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
#include "bloom.h"

#include "params.h"

#include "util/debug.h"

#define bloom_dbg(fmt,...) do { dbg("bloom: "fmt, __VA_ARGS__); } while (0)

/*
 *
 *
 * definitions
 *
 *
 */

/**
 * Number of bits in a bloom filter
 */
#define BLOOM_BITS (sizeof(bloom)*8)


/*
 *
 *
 * forward declarations
 *
 *
 */

/**
 * Special function only to be used in bloom_may_have_common()
 *
 * This function will generate a mask to erase each other virtual integer of
 * a given width within another int.
 *
 * @memberof bloom
 * @return mask to be used to mask ints
 */
__r_const__ static bloom
erasure_mask(
    unsigned int width //!< width of ints to mask
);


/*
 *
 *
 * interface implementation
 *
 *
 */

bloom
bloom_from_hash(
    r_hash hash
) {
    bloom_dbg("Generate bloom from hash 0x%zx", hash);
    bloom result = 0;

    unsigned int vars = HASH_VARIANTS;
    while (vars--) {
        result |= 1 << (hash % BLOOM_BITS);

        // transform the hash to generate new variant
        hash /= BLOOM_BITS;
    }

    bloom_dbg("Generated bloom 0x%zx from hash 0x%zx", bloom, hash);
    return result;
}

int
bloom_may_contain(
    bloom element,
    bloom set
) {
    return (element & (~set)) == 0;
}

int
bloom_may_have_common(
    bloom a,
    bloom b
) {
    // calculate the number of bits common to both filters
    bloom section = a & b;

    // consider "section" to be made up by ints of single bits
    unsigned int width = 1;

    while (width < BLOOM_BITS) {
        // we need a mask to mask out half of the ints
        bloom mask = erasure_mask(width);

        // manually add pairs of ints, clearing half of them to prevent overflows
        section = ((section >> width) & mask) + (section & mask);

        // the resulting ints are double the width (and half in numbers)
        width <<= 1;
    }

    // the resulting number may now be compared to the number of hash variants
    return section < HASH_VARIANTS;
}

/*
 *
 *
 * implementation of internal functions
 *
 *
 */


static bloom
erasure_mask(
    unsigned int width
) {
    // we select the 2nd integer, starting from the least significant
    bloom mask = ((((size_t) 1) << width) - 1) << width;

    // we add that pattern until the mask is filled
    for (unsigned int pos = BLOOM_BITS / (2 * width); pos; --pos) {
        mask |= mask << (2 * width);
    }

    return mask;
}


