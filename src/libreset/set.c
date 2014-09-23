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

#include <stdlib.h>

#include "libreset/set.h"

#include "ht.h"
#include "util/likely.h"

struct r_set {
    struct ht ht;
    const struct r_set_cfg* cfg;
};

struct r_set*
r_set_new(
    struct r_set_cfg const* cfg
) {
    /*
     * magic constant: We initialize the hashtable with 8 buckets, 2^3 == 8, so
     * we must set `ht_init_power` to 3
     */
    const size_t ht_init_power = 3;

    struct r_set* set = calloc(1, sizeof(*set));

    if (likely(set)) {
        if (!ht_init(&set->ht, ht_init_power)) {
            free(set);
            set = NULL;
        } else {
            set->cfg = cfg;
        }
    }

    return set;
}

void
r_set_destroy(
    struct r_set* set
) {
    ht_destroy(&set->ht, set->cfg);
    free(set);
}

int
r_set_insert(
    struct r_set* set,
    void* value
) {
    return ht_insert(&set->ht, value, set->cfg);
}
