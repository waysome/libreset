#ifndef __RS_HASHTABLE__
#define __RS_HASHTABLE__

#include <stdlib.h>

/**
 * @brief hashtable type. We don't show our implementation
 */
struct rs_ht;

// if on 64bit
typedef uint64_t rs_ht_bucket_id;
// else
// typedef uint32_t rs_ht_bucket_id;
// else
// typedef uint16_t rs_ht_bucket_id;
// else
// typedef uint8_t rs_ht_bucket_id;
//
typedef rs_ht_bucket_id (*rs_ht_hashfunc)(void *, size_t);

/*
 * creation and destroying
 */

struct rs_ht *      rs_ht_new        (void);
void                rs_ht_init       (struct rs_ht *ht,
                                     rs_ht_bucket_id size);
struct rs_ht *      rs_ht_destroy    (struct rs_ht *ht);

/*
 * configuration
 */

void rs_ht_set_hasherfunc   (struct rs_ht *ht, rs_ht_hashfunc f);

/*
 * usage
 */

rs_ht_bucket_id     rs_ht_put    (struct rs_ht *ht, void *d, size_t ds);
void *              rs_ht_get    (struct rs_ht *ht, rs_ht_bucket_id);
void *              rs_ht_del    (struct rs_ht *ht, rs_ht_bucket_id);

/*
 * metainformations
 */

rs_ht_bucket_id     rs_ht_size       (struct rs_ht *ht);
float               rs_ht_load       (struct rs_ht *ht);
int                 rs_ht_is_empty   (struct rs_ht *ht);

#endif //__RS_HASHTABLE__

