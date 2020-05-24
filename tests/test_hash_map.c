#include <minimalist/hash_map.h>

#ifndef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <string.h>

size_t hash_string(const void *x) {
  size_t hash = 0;
  int i = 0;
  const char* str = x;
  int str_len = strlen(x);

  for (i = 0; i < str_len; i++) {
    hash = hash * 31 + str[i];
  }
  return hash;
}

int compare_strings(const void *a, const void *b) {
  const char *str_a = a, *str_b = b;
  return strcmp(str_a, str_b);
}

int main() {
  struct minimalist_hash_map* map = NULL;
  map = minimalist_hash_map_new(32, NULL, NULL);
  assert(map == NULL); 
  map = minimalist_hash_map_new(32, hash_string, compare_strings);
  assert(map != NULL);
  char* value = "value";
  minimalist_hash_map_set(map, "keya", NULL);
  minimalist_hash_map_set(map, "keyb", value);
  minimalist_hash_map_set(map, "keyc", NULL);
  minimalist_hash_map_set(map, "keyd", NULL);
  char* ret = minimalist_hash_map_get(map, "keyb");
  assert(value == ret);
  minimalist_hash_map_free(map);
  return 0;
}
