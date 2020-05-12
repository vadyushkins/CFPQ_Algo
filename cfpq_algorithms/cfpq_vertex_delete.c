#include "algorithms.h"

void cfpq_vertex_delete(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v) {
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

    GrB_Index v_id = ItemMapper_Find((ItemMapper*) &deletion.nodes, v);
    
    for (GrB_Index to_id = 0; to_id < deletion.nodes.count; ++to_id) {
        for (GrB_Index edge_id = 0; edge_id < deletion.edges.count; ++edge_id) {
            char* to = ItemMapper_Map((ItemMapper*) &deletion.nodes, to_id);
            char* edge = ItemMapper_Map((ItemMapper*) &deletion.edges, edge_id);

            bool cur = false;

            GrB_Matrix_extractElement_BOOL(&cur, graph->terminal_matrices[edge_id], v_id, to_id);
            if (cur == true) {
                GrB_Matrix_setElement_BOOL(deletion.terminal_matrices[edge_id], true, v_id, to_id);
            }

            GrB_Matrix_extractElement_BOOL(&cur, graph->terminal_matrices[edge_id], to_id, v_id);
            if (cur == true) {
                GrB_Matrix_setElement_BOOL(deletion.terminal_matrices[edge_id], true, to_id, v_id);
            }
        }
    }

    cfpq_dynamic_deletion(graph, grammar, response, &deletion);
}