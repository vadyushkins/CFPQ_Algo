#include "algorithms.h"

void cfpq_vertex_delete(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v) {

    GrB_Index v_id = ItemMapper_Find((ItemMapper*) &graph->nodes, v);
    
    for (GrB_Index to_id = 0; to_id < graph->nodes.count; ++to_id) {
        for (GrB_Index edge_id = 0; edge_id < graph->edges.count; ++edge_id) {
            char* to = ItemMapper_Map((ItemMapper*) &graph->nodes, to_id);
            char* edge = ItemMapper_Map((ItemMapper*) &graph->edges, edge_id);

            bool cur = false;

            GrB_Matrix_extractElement_BOOL(&cur, graph->terminal_matrices[edge_id], v_id, to_id);
            if (cur == true) {
                cfpq_edge_deleted(graph, grammar, response, v_id, edge_id, to_id);
            }

            GrB_Matrix_extractElement_BOOL(&cur, graph->terminal_matrices[edge_id], to_id, v_id);
            if (cur == true) {
                cfpq_edge_deleted(graph, grammar, response, v_id, edge_id, to_id);
            }
        }
    }
}