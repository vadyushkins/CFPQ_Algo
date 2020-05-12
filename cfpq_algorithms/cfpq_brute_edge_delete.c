#include "algorithms.h"

void cfpq_brute_edge_delete(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v, const char* edge, const char* to) {
    GraphRepr_DeleteEdge(graph, v, edge, to);
    cfpq_static(graph, grammar, response);
}