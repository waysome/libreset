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
#include <errno.h>

#include "libreset/set.h"
#include "libreset/hash.h"

#include "common.h"

#include "ht/ht.h"
#include "util/likely.h"

#include "set_type.h"

struct r_set*
r_set_new(
    struct r_set_cfg const* cfg
) {
    set_dbg("Allocate set with config %p", (void*) cfg);
    /*
     * magic constant: We initialize the hashtable with 8 buckets, 2^3 == 8, so
     * we must set `ht_init_power` to 3
     */
    const size_t ht_init_power = 3;

    struct r_set* set = calloc(1, sizeof(*set));

    if (likely(set)) {
        if (!ht_init(&set->ht, ht_init_power)) {
            set_dbg("Allocation failed: %p", (void*)set);
            free(set);
            set = NULL;
        } else {
            set->cfg = cfg;
        }
    }

    return set;
}

int
r_set_destroy(
    struct r_set* set
) {
    int ret;
    if (set) {
        set_dbg("Destroy set: %p", (void*) set);
        ret = ht_destroy(&set->ht, set->cfg);
        free(set);
    } else {
        return -EEXIST;
    }
    return ret;
}

int
r_set_insert(
    struct r_set* set,
    void* value
) {
    set_dbg("Insert %p into set %p", (void*) value, (void*) set);
    return ht_insert(&set->ht, value, set->cfg);
}

int
r_set_remove(
    struct r_set* set,
    void const* cmp
) {
    set_dbg("Remove with compare element %p from set %p",
            (void*) cmp, (void*) set);
    return ht_del(&set->ht, cmp, set->cfg);
}

void*
r_set_contains(
    struct r_set const* set,
    void const* cmp
) {
    set_dbg("Check whether set %p contains element which compares to %p",
            (void*) set,
            (void*) cmp);
    return ht_find(&set->ht, cmp, set->cfg);
}

size_t
r_set_cardinality(
    struct r_set const* set
) {
    set_dbg("Get cardinality for set %p", (void*) set);
    return ht_cardinality(&set->ht);
}

int
r_set_select(
    struct r_set* src,
    r_predf pred,
    void* pred_etc,
    r_procf procf,
    void* dest
) {
    return ht_select(&src->ht, pred, pred_etc, procf, dest);
}

int
r_set_equal(
    struct r_set const* set_a,
    struct r_set const* set_b
) {
    if (set_a == set_b) {
        return 1;
    }

    if (!(set_a && set_b)) {
        return 0;
    }

    if (!config_cmp(set_a->cfg, set_b->cfg)) {
        return 0;
    }

    return ht_equal(&set_a->ht, &set_b->ht, set_a->cfg);
}

