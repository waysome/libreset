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

#include "ll.h"

void
ll_destroy(
    struct ll* ll,
    struct r_set_cfg* cfg
) {
    struct ll_element* iter = ll->head;
    struct ll_element* next;

    while (iter) {
        next = iter->next;
        if (cfg->freef) {
            cfg->freef(iter->data);
        }
        free(iter);
        iter = next;
    }
}

int
ll_insert(
    struct ll* ll,
    void* data,
    struct r_set_cfg* cfg
) {
    // check whether the lement is present or not
    struct ll_element** it = &ll->head;
    while (*it) {
        if (cfg->cmpf((*it)->data, data)) {
            return 0;
        }

        it = &(*it)->next;
    }

    // insert the new element
    struct ll_element* el = calloc(1, sizeof(struct ll_element));
    if (!el) {
        return 0;
    }

    el->data = data;
    *it = el;

    return 1;
}

void*
ll_find(
    struct ll* ll,
    void* const d,
    struct r_set_cfg* cfg
) {
    struct ll_element* iter = ll->head;

    while (iter) {
        if (cfg->cmpf(iter->data, d)) {
            return iter->data;
        }
        iter = iter->next;
    }

    return NULL;
}

int
ll_delete(
    struct ll* ll,
    void* del,
    struct r_set_cfg* cfg
) {
    struct ll_element** iter = &ll->head;

    // iterate over all the elements
    while (*iter) {
        // check whther we have found the element to remove
        if (cfg->cmpf((*iter)->data, del)) {
            struct ll_element* to_del = (*iter);

            // free, relink and return
            if (cfg->freef) {
                cfg->freef(to_del->data);
            }
            *iter = to_del->next;
            free(to_del);
            return 1;
        }

        // iterate further
        iter = &(*iter)->next;
    }

    return 0;
}

unsigned int
ll_ndel(
    struct ll* ll,
    r_predf pred,
    void* etc,
    struct r_set_cfg* cfg
) {
    struct ll_element** iter = &ll->head;
    unsigned int cnt = 0;

    // iterate over all the elements
    while (*iter) {
        // check whther we have found an element to remove
        if (!pred((*iter)->data, etc)) {
            // keep this element -> iterate
            iter = &(*iter)->next;
            continue;
        }

        // remove this element
        struct ll_element* to_del = (*iter);

        // free, relink and increment the counter
        if (cfg->freef) {
            cfg->freef(to_del->data);
        }
        *iter = to_del->next;
        free(to_del);
        ++cnt;
    }

    return cnt;
}

int
ll_is_empty(
    struct ll* ll
) {
    return !ll->head;
}

