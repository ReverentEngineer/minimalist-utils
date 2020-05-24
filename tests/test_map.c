#include <minimalist/map.h>

#ifndef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int compare_strings(const void *a, const void *b) {
  const char *str_a = a, *str_b = b;
  return strcmp(str_a, str_b);
}

static int run_count = 0;

static void run_fn(void* context, const void* key, void* value) {
  run_count++;
}

int main() {
  struct minimalist_map* map = NULL;
  map = minimalist_map_new(compare_strings);
  assert(map != NULL);
  
  char* value = "value";
  minimalist_map_set(map, "keya", NULL);
  minimalist_map_set(map, "keyb", value);
  minimalist_map_set(map, "keyc", NULL);
  minimalist_map_set(map, "keyd", NULL);
  
  char* ret = minimalist_map_get(map, "keyb");
  assert(value == ret);

  ret = minimalist_map_get(map, "a");
  assert(ret == NULL);

  minimalist_map_run(map, run_fn, NULL);
  assert(run_count == 4);

  minimalist_map_keys_t keys;

  int num_keys = minimalist_map_keys(map, &keys);
  assert(num_keys == 4);

  minimalist_map_free(map);
  return 0;
}
