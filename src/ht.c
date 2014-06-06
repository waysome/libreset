#include "libreset/ht.h"

#include <stdlib.h>

struct rs_ht
{
    rs_ht_hashfunc hashfunc;
    rs_ht_bucket_id length;
    void **buckets;
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

struct rs_ht *
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
}

void *
rs_ht_get(struct rs_ht *ht, rs_ht_bucket_id)
{
}

void *
rs_ht_del(struct rs_ht *ht, rs_ht_bucket_id)
{
}


/*
 * metainformations
 */

rs_ht_bucket_id
rs_ht_size(struct rs_ht *ht)
{
}

float
rs_ht_load(struct rs_ht *ht)
{
}

int
rs_ht_is_empty(struct rs_ht *ht)
{
}
