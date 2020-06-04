#include "algorithms.h"

void cfpq_brute_vertex_deleted(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v) {
    GrB_Index v_id = atoll(v);
    
    for (GrB_Index to_id = 0; to_id < graph->nodes_count; ++to_id) {
        for (GrB_Index edge_id = 0; edge_id < grammar->terminals.count; ++edge_id) {
            char* edge = ItemMapper_Map((ItemMapper*) &grammar->terminals, edge_id);

            bool cur = false;

            GrB_Matrix_extractElement_BOOL(&cur, graph->terminal_matrices[edge_id], v_id, to_id);
            if (cur == true) {
                GraphRepr_DeleteEdge(graph, grammar, v, edge, to_id + '0');
            }

            GrB_Matrix_extractElement_BOOL(&cur, graph->terminal_matrices[edge_id], to_id, v_id);
            if (cur == true) {
                GraphRepr_DeleteEdge(graph, grammar, to_id + '0', edge, v);
            }
        }
    }

    cfpq_static(graph, grammar, response);
}