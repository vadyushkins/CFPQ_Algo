#include "algorithms.h"

void cfpq_edge_add(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v, const char* edge, const char* to) {
    GraphRepr addition;
    GraphRepr_Init(&addition);

    for (GrB_Index i = 0; i < graph->nodes.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &addition.nodes, 
            ItemMapper_Map((ItemMapper*) &graph->nodes, i)
        );
    }

    for (GrB_Index i = 0; i < graph->edges.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &addition.edges, 
            ItemMapper_Map((ItemMapper*) &graph->edges, i)
        );
    }

    GraphRepr_InsertEdge(&addition, v, edge, to);

    cfpq_dynamic_addition(graph, grammar, response, &addition);
}