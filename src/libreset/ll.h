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
#ifndef __LL_H__
#define __LL_H__

/**
 *
 */
struct ll_element {
    struct ll_element*  next;
    void*               data;
};

struct ll {
    struct ll_element* head;
};

void
ll_destroy(struct ll* ll);

struct ll*
ll_insert(struct ll* ll, struct ll_element* e);

struct ll*
ll_insert_data(struct ll* ll, void* data);

struct ll*
ll_delete(struct ll* ll, struct ll_element* del);

#define ll_foreach(it,ll) \
    for (struct ll_element* it = ll->head; it; it = it->next)

struct ll_element*
ll_element_alloc(void);

struct ll_element*
ll_element_alloc_new(void* data);

void
ll_element_destroy(struct ll_element*);

#endif
