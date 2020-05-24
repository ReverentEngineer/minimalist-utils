#ifndef __MINIMALIST_MAP_H__
#define __MINIMALIST_MAP_H__
/**
 * @file map.h
 * @brief A map implementation using a red-black tree
 */

#include <minimalist/types.h>

/**
 * @brief A red-black map
 */
struct minimalist_map;

/**
 * @brief A run callback
 */
typedef void (*minimalist_map_run_fn)(void *context,
                                      const void *key,
                                      void *value);

typedef const void **minimalist_map_keys_t;

/**
 * @brief Creates a new red-black map
 *
 * If compare is NULL, the addresses are compared.
 *
 * @param compare The comparison method use for keys
 *
 * @return An instance of a red-black map
 */
struct minimalist_map *minimalist_map_new(minimalist_const_compare_fn compare);

/**
 * @brief Frees an instance of a red-black map.
 *
 * @param map Map to free
 */
void minimalist_map_free(struct minimalist_map *map);

/**
 * @brief Sets an element in a map.
 *
 * Setting the value to NULL removes any previously set element.
 *
 * @param map The map on which to operate.
 * @param key The key to use for the element.
 * @param value The value of the element.
 */
void
minimalist_map_set(struct minimalist_map *map, const void *key, void *value);

/**
 * @brief Gets element in a map
 *
 * @param map The map to sarch
 * @param key The key of the element.
 *
 * @return The element, if found. Otherwise, NULL.
 */
void *minimalist_map_get(struct minimalist_map *map, const void *key);

/**
 * @brief Runs function on each value in map
 *
 * @param map The map
 * @param run The function to run on the values
 * @param context A context for function
 *
 */
void minimalist_map_run(struct minimalist_map *map,
                        minimalist_map_run_fn run,
                        void *context);

/**
 * @brief Returns all the keys in a map
 *
 * @param map The map
 * @param keys A pointer to an allocated array of map keys.
 *
 * @note keys will be NULL if function returns 0
 *
 * @return Number of keys
 */
int minimalist_map_keys(struct minimalist_map *map,
                        minimalist_map_keys_t *keys);

#endif /* __MINIMALIST_MAP_H__ */
