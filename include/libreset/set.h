#ifndef __LIBRESET_H__
#define __LIBRESET_H__

struct r_set;

struct r_set_cfg
{
    size_t  (*hashf)(void* const);
    int     (*cmpf)(void* const, void* const);
};


struct r_set*
r_set_alloc(
        struct r_set_cfg const* cfg
);


void
r_set_free(
        struct r_set* set
);


int
r_set_insert(
        struct r_set* set,
        void const* value
);


void* const
r_set_remove(
        struct r_set* set,
        void const* cmp
);


void* const
r_set_contains(
        struct r_set* set,
        void const* cmp
);


int
r_set_union(
        struct r_set* dest,
        struct r_set* set_a,
        struct r_set* set_b,
);


int
r_set_intersection(
        struct r_set* dest,
        struct r_set* set_a,
        struct r_set* set_b,
);


int
r_set_xor(
        struct r_set* dest,
        struct r_set* set_a,
        struct r_set* set_b,
);


int
r_set_exclude(
        struct r_set* dest,
        struct r_set* set_a,
        struct r_set* set_b,
);


int
r_set_is_subset(
        struct r_set* set_a,
        struct r_set* set_b,
);


int
r_set_equal(
        struct r_set* set_a,
        struct r_set* set_b,
);


size_t
r_set_cardinality(
        struct r_set* set
);

#endif //__LIBRESET_H__
