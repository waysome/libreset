#ifndef __RS_HASHTABLE__
#define __RS_HASHTABLE__

#include <stdlib.h>
#include <stdint.h>

/**
 * @brief hashtable type. We don't show our implementation
 */
struct rs_ht;

// if on 64bit
typedef uint64_t rs_ht_hash;
// else
// typedef uint32_t rs_ht_hash;
// else
// typedef uint16_t rs_ht_hash;
// else
// typedef uint8_t rs_ht_hash;
//
typedef rs_ht_hash (*rs_ht_hashfunc)(void *, size_t);

/*
 * creation and destroying
 */

/**
 * @brief Allocate a new (uninitialized) hashtable
 *
 * @return A new hashtable object or NULL on failure
 */
struct rs_ht *      rs_ht_new        (void);

/**
 * @brief Initialize a allocated hashtable object
 *
 * This function allocates the internal bucket array.
 *
 * @param ht The hashtable to initialize
 * @param size The size of the internal bucket array
 */
void                rs_ht_init       (struct rs_ht *ht,
                                     rs_ht_hash size);

/**
 * @brief Destroy a hashtable object
 *
 * @param ht The hashtable to destory
 */
void                rs_ht_destroy    (struct rs_ht *ht);

/*
 * configuration
 */

/**
 * @brief Set the hashing function for the hashtable
 *
 * @param ht The hashtable
 * @param f The hashing function
 */
void rs_ht_set_hasherfunc   (struct rs_ht *ht, rs_ht_hashfunc f);

/*
 * usage
 */

/**
 * @brief Put something into the hashtable
 *
 * @param ht The hashtable to put into
 * @param d The data
 * @param ds The size of the data
 *
 * @return The hash generated by the hasher function
 */
rs_ht_hash          rs_ht_put    (struct rs_ht *ht, void *d, size_t ds);

/**
 * @brief Get something by its hash
 *
 * @param ht The hashtable
 * @param rs_ht_hash The hash
 *
 * @return The data object
 */
void *              rs_ht_get    (struct rs_ht *ht, rs_ht_hash);

/**
 * @brief Get the size of an element by its hash
 *
 * @param ht The hashtable
 * @param rs_ht_hash The hash
 *
 * @return The size of the element with the passed hash, zero if not found.
 */
size_t              rs_ht_getsize(struct rs_ht *ht, rs_ht_hash);

/**
 * @brief Delete something by its hash
 *
 * @param ht The hashtable
 * @param rs_ht_hash The hash
 *
 * @return The data object, is now removed from the hashtable
 */
void *              rs_ht_del    (struct rs_ht *ht, rs_ht_hash);


/**
 * @brief Map one hashtable into another
 *
 * @param ht The hastable to map
 * @param other The other (initialized) hashtable
 * @param factor The factor, usually (other->length / ht->length), multiple of 2
 *
 * @return zero on success
 */
int                 rs_ht_map_on    (struct rs_ht *ht,
                                    struct rs_ht *other,
                                    unsigned int factor);

/*
 * metainformations
 */

/**
 * @brief Get the size (number of values inside) the hashtable
 *
 * @param ht The hashtable
 *
 * @return The size of the hashtable
 */
rs_ht_hash          rs_ht_size       (struct rs_ht *ht);

/**
 * @brief Get the load of the hashtable
 *
 * @param ht The hashtable
 *
 * @return The load
 */
float               rs_ht_load       (struct rs_ht *ht);

/**
 * @brief Get if the hashtable is empty
 *
 * @param ht The hashtable
 *
 * @return 1 if it is empty, else zero
 */
int                 rs_ht_is_empty   (struct rs_ht *ht);

#endif //__RS_HASHTABLE__

