#include "minimalist/map.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum color_t { RED, BLACK };

struct map_node {
  struct map_node *parent;
  struct map_node *left;
  struct map_node *right;
  enum color_t color;
  const void *key;
  void *value;
};

static int
address_compare(const void *a, const void *b) {
  return b - a;
}

struct minimalist_map {
  struct map_node *root;
  minimalist_const_compare_fn compare;
};

struct minimalist_map *
minimalist_map_new(minimalist_const_compare_fn compare) {

  struct minimalist_map *map = NULL;

  map = malloc(sizeof(struct minimalist_map));

  if (map == NULL) {
    goto err;
  }

  if (compare) {
    map->compare = compare;
  } else {
    map->compare = address_compare;
  }
  map->root = NULL;
err:
  return map;
}

static void
free_node(struct map_node *node) {
  if (node->right != NULL) {
    free_node(node->right);
  } else if (node->left != NULL) {
    free_node(node->left);
  }
  free(node);
}

void
minimalist_map_free(struct minimalist_map *map) {
  if (map) {
    if (map->root) {
      free_node(map->root);
    }
    free(map);
  }
}

static struct map_node *
get_sibling(struct map_node *node) {
  if (node->parent == NULL) {
    return NULL;
  }

  if (node == node->parent->left) {
    return node->parent->right;
  } else {
    return node->parent->left;
  }
}

static struct map_node *
get_parent(struct map_node *node) {
  return node == NULL ? NULL : node->parent;
}

static struct map_node *
get_grand_parent(struct map_node *node) {
  return get_parent(get_parent(node));
}

static struct map_node *
get_uncle(struct map_node *node) {
  struct map_node *parent = get_parent(node);
  return get_sibling(parent);
}

static void
rotate_left(struct map_node *node) {
  struct map_node *new_node = node->right;
  struct map_node *parent = get_parent(node);
  assert(new_node != NULL);

  node->right = new_node->left;
  new_node->left = node;
  if (node->right != NULL) {
    node->right->parent = node;
  }

  if (parent != NULL) {
    if (node == parent->left) {
      parent->left = new_node;
    } else if (node == parent->right) {
      parent->right = new_node;
    }
  }
}

static void
repair(struct map_node *node) {
  if (get_parent(node) == NULL) {
    node->color = BLACK;
  } else if (get_parent(node)->color == BLACK) {
    // Do nothing
  } else if (get_uncle(node) != NULL && get_uncle(node)->color == RED) {
    get_parent(node)->color = BLACK;
    get_uncle(node)->color = BLACK;
    get_grand_parent(node)->color = RED;
    repair(get_grand_parent(node));
  } else {
    // TODO: Balance map
  }
}

static void
add(struct map_node *root,
    struct map_node *node,
    minimalist_const_compare_fn compare) {
  int comparison = compare(root->key, node->key);
  if (root != NULL) {
    if (comparison > 0) {
      if (root->right != NULL) {
        add(root->right, node, compare);
      } else {
        node->parent = root;
        root->right = node;
      }
    } else if (comparison < 0) {
      if (root->left != NULL) {
        add(root->left, node, compare);
      } else {
        node->parent = root;
        root->left = node;
      }
    } else {
      root->value = node->value;
      free(node);
    }
  }
}

void
minimalist_map_set(struct minimalist_map *map, const void *key, void *value) {
  struct map_node *new_node = malloc(sizeof(struct map_node));
  if (new_node != NULL) {
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->key = key;
    new_node->value = value;
    new_node->color = RED;
    if (map->root != NULL) {
      add(map->root, new_node, map->compare);
    } else {
      map->root = new_node;
    }
    repair(new_node);
  }
}

static void *
find(struct map_node *node,
     const void *key,
     minimalist_const_compare_fn compare) {
  if (node) {
    int comparison = compare(node->key, key);
    if (comparison > 0) {
      return find(node->right, key, compare);
    } else if (comparison < 0) {
      return find(node->left, key, compare);
    } else {
      return node->value;
    }
  } else {
    return NULL;
  }
}

void *
minimalist_map_get(struct minimalist_map *map, const void *key) {
  return find(map->root, key, map->compare);
}

static void
map_node_run(struct map_node *node, minimalist_map_run_fn run, void *context) {
  // Run left-to-right
  if (node->left != NULL) {
    map_node_run(node->left, run, context);
  }
  run(context, node->key, node->value);
  if (node->right != NULL) {
    map_node_run(node->right, run, context);
  }
}

void
minimalist_map_run(struct minimalist_map *map,
                   minimalist_map_run_fn run,
                   void *context) {
  if (run && map->root) {
    map_node_run(map->root, run, context);
  }
}

static void
get_keys(struct map_node *node, const void ***keys, int *num_keys) {
  if (node->left) {
    get_keys(node->left, keys, num_keys);
  }
  (*num_keys)++;
  *keys = realloc(*keys, sizeof(void *) * (*num_keys));
  (*keys)[(*num_keys) - 1] = node->key;
  if (node->right) {
    get_keys(node->right, keys, num_keys);
  }
}

int
minimalist_map_keys(struct minimalist_map *map, const void ***keys) {
  *keys = NULL;
  int num_keys = 0;
  if (map->root) {
    get_keys(map->root, keys, &num_keys);
  }
  return num_keys;
}
