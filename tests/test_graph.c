#include "minimalist/graph.h"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdlib.h>

char* a = "A";
char* b = "B";
char* c = "C";
char* d = "D";

int main() {
  struct minimalist_graph* graph = NULL;
  graph =  minimalist_graph_new(0);
  minimalist_graph_add_edge(graph, a, b);
  minimalist_graph_add_edge(graph, b, c);
  minimalist_graph_add_edge(graph, b, d);
  assert(!minimalist_graph_cyclic(graph));
  minimalist_graph_free(graph);
  
  graph =  minimalist_graph_new(0);
  minimalist_graph_add_edge(graph, a, b);
  minimalist_graph_add_edge(graph, b, c);
  minimalist_graph_add_edge(graph, c, d);
  minimalist_graph_add_edge(graph, d, a);
  assert(minimalist_graph_cyclic(graph));


  return 0;
}
