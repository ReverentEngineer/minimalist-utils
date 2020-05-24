#ifndef __MINIMALIST_TYPES_H__
#define __MINIMALIST_TYPES_H__
/**
 * @file types.h
 * @brief Types used throughout the library
 */

/**
 * @brief A compare callback
 */
typedef int (*minimalist_compare_fn)(void *a, void *b);

/**
 * @brief A compare callback
 */
typedef int (*minimalist_const_compare_fn)(const void *a, const void *b);

#endif /* __MINIMALIST_TYPES_H__ */
