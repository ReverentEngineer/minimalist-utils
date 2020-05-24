#ifndef __MINIMALIST_GRAPH_H__
#define __MINIMALIST_GRAPH_H__
/**
 * @file graph.h
 * @brief A graph implementation using adjacency lists
 */

/** @brief A graph **/
struct minimalist_graph;

/** @brief List of neighbors */
typedef void **minimalist_graph_neighbor_list_t;

/** @brief List of paths */
typedef void ***minimalist_graph_path_list_t;

/**
 * @brief Creates a new graph;
 *
 * @param directed Whether graph is directed
 *
 * @return A pointer to a graph
 */
struct minimalist_graph *minimalist_graph_new(int directed);

/**
 * @brief Frees a graph and all nodes associated with it
 *
 * @param graph Graph to free
 */
void minimalist_graph_free(struct minimalist_graph *graph);

/**
 * @brief Adds an edge to the graph
 *
 * If the graph is directed, the direction is a to b.
 *
 * @param graph Graph
 * @param a First node to connect
 * @param b Second node to connect
 */
void
minimalist_graph_add_edge(struct minimalist_graph *graph, void *a, void *b);

/**
 * @brief Get all neighbors of a NULL.
 *
 * @return NULL if node doesn't exist, otherwise a NULL terminated list of
 * pointers to neighbor nodes.
 */
minimalist_graph_neighbor_list_t
minimalist_graph_get_neighbors(struct minimalist_graph *graph, void *node);

/**
 * @brief Gets list of paths between two nodes.
 */
minimalist_graph_path_list_t minimalist_graph_get_paths(
    struct minimalist_graph *graph, void *start, void *end);

/**
 * @brief Test if graph is cyclic
 *
 * @param graph
 */
int minimalist_graph_cyclic(struct minimalist_graph *graph);

#endif /* __MINIMALIST_GRAPH_H__ */
