#ifndef __MINIMALIST_HASH_MAP_H__
#define __MINIMALIST_HASH_MAP_H__
/**
 * @file hash_map.h
 * @brief A hash map implementation
 */

#include <stddef.h>

/**
 * @brief A hash map implementation
 */
struct minimalist_hash_map;

/**
 * @brief A function pointer to a hash function
 */
typedef size_t (*minimalist_hash_map_hash_fn)(const void *);

/**
 * @brief A function pointer to a compare function
 */
typedef int (*minimalist_hash_map_compare_fn)(const void *, const void *);

/**
 * @brief Creates a new hash map
 */
struct minimalist_hash_map *
minimalist_hash_map_new(size_t buckets,
                        minimalist_hash_map_hash_fn hash,
                        minimalist_hash_map_compare_fn compare);

/**
 * @brief Frees the hash map
 */
void minimalist_hash_map_free(struct minimalist_hash_map *map);

/**
 * @brief Sets a new hash map entry
 *
 * @param map
 * @param key
 * @param value
 */
void minimalist_hash_map_set(struct minimalist_hash_map *map,
                             const void *key,
                             void *value);

/**
 * @brief Gets an existing hash map entry
 *
 * @param map
 * @param key
 *
 * @return Value stored in entry.
 */
void *minimalist_hash_map_get(struct minimalist_hash_map *map, const void *key);

#endif /* __MINIMALIST_HASH_MAP_H__ */
