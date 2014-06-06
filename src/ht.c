#include "libreset/ht.h"

#include <stdlib.h>

struct ll_element {
    struct ll_element   *next;
    void                *value;
    size_t              vsize;
};

struct rs_ht
{
    rs_ht_hashfunc hashfunc;
    rs_ht_bucket_id length;
    struct ll_element **buckets;
};

/*
 * creation and destroying
 */

struct rs_ht *
rs_ht_new()
{
    return calloc(1, sizeof(struct rs_ht));
}

void
rs_ht_init(struct rs_ht *ht, rs_ht_bucket_id size)
{
    ht->hashfunc = NULL;
    ht->length = size;
    ht->buckets = calloc(ht->length, sizeof(*ht->buckets));
}

void
rs_ht_destroy(struct rs_ht *ht)
{
    free(ht->buckets);
    free(ht);
}


/*
 * configuration
 */

void
rs_ht_set_hasherfunc(struct rs_ht *ht, rs_ht_hashfunc f)
{
    ht->hashfunc = f;
}

/*
 * usage
 */

rs_ht_bucket_id
rs_ht_put(struct rs_ht *ht, void *d, size_t ds)
{
    rs_ht_bucket_id hash = 0;
    rs_ht_bucket_id dest = 0;
    struct ll_element *el = malloc(sizeof(*el));
    struct ll_element **iter;

    if (el) {
        el->value = d;
        el->vsize = ds;
        el->next = NULL;

        hash = ht->hashfunc(d, ds);
        dest = hash % ht->length;

        iter = &ht->buckets[dest];

        for(; *iter; iter = &(*iter)->next);

        *iter = el;
    }
    else {
        // TODO: ERROR
    }

    return hash;
}

void *
rs_ht_get(struct rs_ht *ht, rs_ht_bucket_id hash)
{
    struct ll_element *bucket = ht->buckets[hash % ht->length];
    void *v = NULL;

    for (; !v && bucket; bucket = bucket->next) {
        if (hash == ht->hashfunc(bucket->value, bucket->vsize)) {
            v = bucket->value;
            break;
        }
    }

    return v;
}

void *
rs_ht_del(struct rs_ht *ht, rs_ht_bucket_id hash)
{
    rs_ht_bucket_id pos = hash % ht->length;
    struct ll_element *prev = NULL;
    struct ll_element *el_iter = ht->buckets[pos];
    void *d = NULL;

    for(; !d && el_iter; el_iter = el_iter->next) {
        if (hash == ht->hashfunc(el_iter->value, el_iter->vsize)) {
            if (el_iter->next) {
                if (prev) {
                    prev->next = el_iter->next;
                }
                else {
                    ht->buckets[pos] = el_iter->next;
                }
            }

            d = el_iter->value;
            free(el_iter);
            break;
        }

        prev = el_iter;
    }

    return d;
}


/*
 * metainformations
 */

rs_ht_bucket_id
rs_ht_size(struct rs_ht *ht)
{
    rs_ht_bucket_id s = 0;
    rs_ht_bucket_id i;

    for(i = 0; i < ht->length; i++) {
        if (ht->buckets[i]) {
            s++;
        }
    }

    return s;
}

float
rs_ht_load(struct rs_ht *ht)
{
    // TODO
}

int
rs_ht_is_empty(struct rs_ht *ht)
{
    return 0 == rs_ht_size(ht);
}
