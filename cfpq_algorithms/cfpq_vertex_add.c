#include "algorithms.h"

void cfpq_vertex_add(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v) {
    ItemMapper_Insert((ItemMapper*) &graph->nodes, v);
}