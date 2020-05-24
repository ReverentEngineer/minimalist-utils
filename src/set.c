#include "minimalist/set.h"

#include <assert.h>
#include <stdlib.h>

enum color_t { RED, BLACK };

struct set_node {
  struct set_node *parent;
  struct set_node *left;
  struct set_node *right;
  enum color_t color;
  const void *value;
};

static int
address_compare(const void *a, const void *b) {
  return b - a;
}

struct minimalist_set {
  struct set_node *root;
  minimalist_const_compare_fn compare;
};

struct minimalist_set *
minimalist_set_new(minimalist_const_compare_fn compare) {

  struct minimalist_set *set = NULL;

  set = malloc(sizeof(struct minimalist_set));

  if (set) {

    if (compare) {
      set->compare = compare;
    } else {
      set->compare = address_compare;
    }
    set->root = NULL;
  }

  return set;
}

static void
free_node(struct set_node *node) {
  if (node->right != NULL) {
    free_node(node->right);
  } else if (node->left != NULL) {
    free_node(node->left);
  }
  free(node);
}

void
minimalist_set_free(struct minimalist_set *set) {
  if (set) {
    if (set->root) {
      free_node(set->root);
    }
    free(set);
  }
}

static struct set_node *
get_sibling(struct set_node *node) {
  if (node->parent == NULL) {
    return NULL;
  }

  if (node == node->parent->left) {
    return node->parent->right;
  } else {
    return node->parent->left;
  }
}

static struct set_node *
get_parent(struct set_node *node) {
  return node == NULL ? NULL : node->parent;
}

static struct set_node *
get_grand_parent(struct set_node *node) {
  return get_parent(get_parent(node));
}

static struct set_node *
get_uncle(struct set_node *node) {
  struct set_node *parent = get_parent(node);
  return get_sibling(parent);
}

static void
rotate_left(struct set_node *node) {
  struct set_node *new_node = node->right;
  struct set_node *parent = get_parent(node);
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
repair(struct set_node *node) {
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
    // TODO: Balance set
  }
}

static void
add(struct set_node *root,
    struct set_node *node,
    minimalist_const_compare_fn compare) {
  int comparison = compare(root->value, node->value);
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
minimalist_set_add(struct minimalist_set *set, const void *value) {
  struct set_node *new_node = malloc(sizeof(struct set_node));
  if (new_node != NULL) {
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->value = value;
    new_node->color = RED;
    if (set->root != NULL) {
      add(set->root, new_node, set->compare);
    } else {
      set->root = new_node;
    }
    repair(new_node);
  }
}

static int
exists(struct set_node *node,
       const void *value,
       minimalist_const_compare_fn compare) {
  if (node) {
    int comparison = compare(node->value, value);
    if (comparison > 0) {
      return exists(node->right, value, compare);
    } else if (comparison < 0) {
      return exists(node->left, value, compare);
    } else {
      return 1;
    }
  } else {
    return 0;
  }
}

int
minimalist_set_exists(struct minimalist_set *set, const void *value) {
  return exists(set->root, value, set->compare);
}

static void
set_node_run(struct set_node *node, minimalist_set_run_fn run, void *context) {
  // Run left-to-right
  if (node->left != NULL) {
    set_node_run(node->left, run, context);
  }
  run(context, node->value);
  if (node->right != NULL) {
    set_node_run(node->right, run, context);
  }
}

void
minimalist_set_run(struct minimalist_set *set,
                   minimalist_set_run_fn run,
                   void *context) {
  assert(run);
  set_node_run(set->root, run, context);
}
