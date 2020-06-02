#include "interpreter.h"

void interprete_queries_without_time(
    const GraphRepr* graph, 
    const Grammar* grammar, 
    Response* response,
    const GraphRepr* addition,
    const GraphRepr* deletion, 
    FILE* f
) {
    char* line_buf;
    size_t buf_size = 0;

    double summary = 0;

    while (getline(&line_buf, &buf_size, f) != -1) {
        line_buf[strcspn(line_buf, "\n")] = 0;

        char type[MAX_COMMAND_LEN];
        char v[MAX_COMMAND_LEN], edge[MAX_COMMAND_LEN], to[MAX_COMMAND_LEN];
        int nitems = sscanf(line_buf, "%s %s %s %s", type, v, to, edge);
        assert(nitems >= 2);

        if (nitems == 2) {
            if (strcmp(type, "brute-vertex-add") == 0) {
                ItemMapper_Insert((ItemMapper*) &graph->nodes, v);
            } else if (strcmp(type, "smart-vertex-add") == 0) {
                ItemMapper_Insert((ItemMapper*) &graph->nodes, v);
            } else if (strcmp(type, "brute-vertex-delete") == 0) {
                cfpq_brute_vertex_delete(graph, grammar, response, v);
            } else if (strcmp(type, "smart-vertex-delete") == 0) {
                cfpq_vertex_delete(graph, grammar, response, v);
            }
        } else if (nitems == 3) {
            if (strcmp(type, "find-path") == 0) {
                GrB_Index v_id = ItemMapper_GetPlaceIndex((ItemMapper*) &graph->nodes, v);
                GrB_Index to_id = ItemMapper_GetPlaceIndex((ItemMapper*) &graph->nodes, to);

                bool result = false;

                GrB_Matrix_extractElement_BOOL(&result, response->nonterminal_matrices[0], v_id, to_id);
            }
        } else if (nitems == 4) {
            if (strcmp(type, "brute-edge-add") == 0) {
                GraphRepr_InsertEdge(graph, v, edge, to);
                cfpq_static(graph, grammar, response);
            } else if (strcmp(type, "smart-edge-add") == 0) {
                GraphRepr_InsertEdge(addition, v, edge, to);
                GrB_Index edge_id = ItemMapper_GetPlaceIndex((ItemMapper*) &graph->edges, edge);
                cfpq_dynamic_addition(graph, grammar, response, addition);
                GrB_Matrix_clear(addition->terminal_matrices[edge_id]);
            } else if (strcmp(type, "brute-edge-delete") == 0) {
                GraphRepr_DeleteEdge(graph, v, edge, to);
                cfpq_static(graph, grammar, response);
            } else if (strcmp(type, "smart-edge-delete") == 0) {
                GraphRepr_InsertEdge(deletion, v, edge, to);
                GrB_Index edge_id = ItemMapper_GetPlaceIndex((ItemMapper*) &graph->edges, edge);
                cfpq_dynamic_deletion(graph, grammar, response, deletion);
                GrB_Matrix_clear(deletion->terminal_matrices[edge_id]);
            }
        }
    }
    printf("Summary time= %lf\n", summary);
}