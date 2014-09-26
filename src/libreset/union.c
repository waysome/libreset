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

#include <string.h>
#include <errno.h>

#include "libreset/set.h"

#include "set_type.h"

static inline int
config_cmp(
    struct r_set_cfg const* a, //!< first param for comparison
    struct r_set_cfg const* b  //!< second param for comparison
) {
    return ((a == b) || (a && b && memcmp(a, b, sizeof(*a))));
}

int
r_set_union(
    struct r_set* dest,
    struct r_set const* a,
    struct r_set const* b
) {
    if (!config_cmp(a->cfg, b->cfg) || !config_cmp(dest->cfg, a->cfg)) {
        return -EINVAL;
    }

    return ht_union(dest->ht, a->ht, dest->cfg) &&
        ht_union(dest->ht, b->ht, dest->cfg);
}
