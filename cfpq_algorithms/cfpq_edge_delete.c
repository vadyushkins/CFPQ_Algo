#include "algorithms.h"

void cfpq_edge_delete(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v, const char* edge, const char* to) {
    GraphRepr deletion;
    GraphRepr_Init(&deletion);

    for (GrB_Index i = 0; i < graph->nodes.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &deletion.nodes, 
            ItemMapper_Map((ItemMapper*) &graph->nodes, i)
        );
    }

    for (GrB_Index i = 0; i < graph->edges.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &deletion.edges, 
            ItemMapper_Map((ItemMapper*) &graph->edges, i)
        );
    }

    GraphRepr_InsertEdge(&deletion, v, edge, to);

    cfpq_dynamic_deletion(graph, grammar, response, &deletion);
}