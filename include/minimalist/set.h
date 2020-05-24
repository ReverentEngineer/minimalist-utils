#ifndef __MINIMALIST_SET_H__
#define __MINIMALIST_SET_H__
/**
 * @file set.h
 * @brief A set implementation using a red-black tree
 */

#include <minimalist/types.h>

typedef void (*minimalist_set_run_fn)(void *context, const void *value);

/**
 * @brief A set
 */
struct minimalist_set;

/**
 * @brief Allocates a set structure
 *
 * @note If no compare callback is provided, it default to using the pointer
 * addresses.
 *
 * @param compare A comparison callback to use.
 *
 * @return A pointer to a set structure 
 */
struct minimalist_set *minimalist_set_new(minimalist_const_compare_fn compare);

/**
 * @brief Frees the set structure
 *
 * @param set The set structure
 */
void minimalist_set_free(struct minimalist_set *set);

/**
 * @brief Adds a value to the set
 *
 * @param set The set
 * @param vale The value to add
 */
void minimalist_set_add(struct minimalist_set *set, const void *value);

/**
 * @brief Checks if a value is in the set
 *
 * @param set The set
 * @param value The value to look for
 *
 * @retval 1 if value exists in set
 * @retval 0 if value does not exist in set
 */
int minimalist_set_exists(struct minimalist_set *set, const void *value);

/**
 * @brief Removes an item from the set
 *
 * @param set The set
 * @param value The value to remove
 */
void minimalist_set_remove(struct minimalist_set *set, const void *value);

/**
 * @brief Runs a function with the provied context over all values in the set
 *
 * @param set The set
 * @param run A callback to run over each item
 * @param contex The context to pass to the run callback
 */
void minimalist_set_run(struct minimalist_set *set,
                        minimalist_set_run_fn run,
                        void *context);

#endif /* __MINIMALIST_SET_H__ */
