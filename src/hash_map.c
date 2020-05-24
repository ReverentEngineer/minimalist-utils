#include "minimalist/hash_map.h"

#include <assert.h>
#include <stdlib.h>

struct bucket {
  const void *key;
  void *value;
  struct bucket *next;
};

struct minimalist_hash_map {
  minimalist_hash_map_hash_fn hash;
  minimalist_hash_map_compare_fn compare;
  int num_buckets;
  struct bucket **buckets;
};

struct minimalist_hash_map *
minimalist_hash_map_new(size_t buckets,
                        minimalist_hash_map_hash_fn hash,
                        minimalist_hash_map_compare_fn compare) {
  struct minimalist_hash_map *map = NULL;
  if (hash != NULL && compare != NULL) {
    map = malloc(sizeof(struct minimalist_hash_map));
    map->hash = hash;
    map->compare = compare;
    map->num_buckets = buckets;
    map->buckets = calloc(buckets, sizeof(struct bucket *));
    assert(map->buckets != NULL);
  }
  return map;
}

void
minimalist_hash_map_free(struct minimalist_hash_map *map) {
  int i = 0;
  struct bucket *next = NULL, *tmp = NULL;

  if (map != NULL) {
    if (map->buckets) {
      for (i = 0; i < map->num_buckets; i++) {
        next = map->buckets[i];
        while (next != NULL) {
          tmp = next->next;
          free(next);
          next = tmp;
        }
      }
      free(map->buckets);
    }
  }
}

void
minimalist_hash_map_set(struct minimalist_hash_map *map,
                        const void *key,
                        void *value) {

  size_t hash = 0;
  struct bucket **bucket = NULL;
  struct bucket *tmp = NULL;
  int delete = value == NULL ? 1 : 0;
  int found = 0;

  hash = map->hash(key) % map->num_buckets;
  bucket = &map->buckets[hash];

  while ((*bucket) != NULL) {
    if (map->compare(key, (*bucket)->key) == 0) {
      if (delete) {
        tmp = *bucket;
        *bucket = tmp->next;
        free(tmp);
        break;
      } else {
        (*bucket)->value = value;
        break;
      }
      break;
    }
    bucket = &(*bucket)->next;
  }
  if (!delete &&(*bucket) == NULL) {
    *bucket = malloc(sizeof(struct bucket));
    (*bucket)->key = key;
    (*bucket)->value = value;
    (*bucket)->next = NULL;
  }
}

void *
minimalist_hash_map_get(struct minimalist_hash_map *map, const void *key) {
  size_t hash = 0;
  struct bucket *bucket = NULL;
  void *value = NULL;

  hash = map->hash(key) % map->num_buckets;
  bucket = map->buckets[hash];
  while (bucket != NULL) {
    if (map->compare(key, bucket->key) == 0) {
      value = bucket->value;
      break;
    }
    bucket = bucket->next;
  }

  return value;
}
