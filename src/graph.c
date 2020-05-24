#include "minimalist/graph.h"

#include "minimalist/map.h"
#include "minimalist/set.h"

#include <assert.h>
#include <stdlib.h>

struct adjacency_list {
  int num_neighbors;
  void **neighbors;
};

typedef void **adjacency_list;

struct minimalist_graph {
  int directed;
  struct minimalist_map *adjacency_lists;
};

struct minimalist_graph *
minimalist_graph_new(int directed) {
  struct minimalist_graph *graph = calloc(1, sizeof(struct minimalist_graph));
  if (graph) {
    graph->directed = directed;
    graph->adjacency_lists = minimalist_map_new(NULL);
    if (graph->adjacency_lists == NULL) {
      free(graph);
      graph = NULL;
    }
  }
  return graph;
}

static void
run_free(void *context, const void *key, void *value) {
  if (value) {
    free(value);
  }
}

void
minimalist_graph_free(struct minimalist_graph *graph) {
  if (graph != NULL) {
    if (graph->adjacency_lists) {
      minimalist_map_run(graph->adjacency_lists, run_free, NULL);
    }
    minimalist_map_free(graph->adjacency_lists);
    free(graph);
  }
}

static void
add_neighbor(struct minimalist_graph *graph, void *a, void *b) {
  struct adjacency_list *list = minimalist_map_get(graph->adjacency_lists, a);
  if (list == NULL) {
    list = malloc(sizeof(struct adjacency_list));
    list->num_neighbors = 1;
    list->neighbors = malloc(sizeof(void *) * list->num_neighbors);
    list->neighbors[0] = b;
    minimalist_map_set(graph->adjacency_lists, a, list);
  } else {
    list->num_neighbors++;
    list->neighbors =
        realloc(list->neighbors, sizeof(void *) * list->num_neighbors);
    list->neighbors[list->num_neighbors - 1] = b;
  }
}

void
minimalist_graph_add_edge(struct minimalist_graph *graph, void *a, void *b) {
  add_neighbor(graph, a, b);
  if (!graph->directed) {
    add_neighbor(graph, b, a);
  }
}

static int
dfs(struct minimalist_map *adjacency_lists,
    struct minimalist_set *visited,
    const void *current,
    const void *parent) {

  struct adjacency_list *list = NULL;
  minimalist_set_add(visited, current);
  list = minimalist_map_get(adjacency_lists, current);
  if (list) {
    for (int i = 0; i < list->num_neighbors; i++) {
      if (list->neighbors[i] != parent) {
        if (minimalist_set_exists(visited, list->neighbors[i])) {
          return 1;
        } else {
          if (dfs(adjacency_lists, visited, list->neighbors[i], current) == 1) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

int
minimalist_graph_cyclic(struct minimalist_graph *graph) {
  struct minimalist_set *visited = minimalist_set_new(NULL);

  if (visited) {
    minimalist_map_keys_t nodes = NULL;
    int num_nodes = minimalist_map_keys(graph->adjacency_lists, &nodes);

    for (int i = 0; i < num_nodes; i++) {
      if (!minimalist_set_exists(visited, nodes[i])) {
        if (dfs(graph->adjacency_lists, visited, nodes[i], NULL)) {
          return 1;
        }
      }
    }
    if (nodes != NULL) {
      free(nodes);
    }
    minimalist_set_free(visited);
  }
  return 0;
}
