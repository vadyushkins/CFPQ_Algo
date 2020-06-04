#include "interpreter.h"

void interprete_queries(
    const GraphRepr* graph, 
    const Grammar* grammar, 
    Response* response,
    FILE* f
) {
    // Initialize timer
    double total_time = 0;
    double timer[2];

    // Initialize input
    char* line_buf;
    size_t buf_size = 0;

    while (getline(&line_buf, &buf_size, f) != -1) {
        line_buf[strcspn(line_buf, "\n")] = 0;

        char type[MAX_COMMAND_LEN];
        char v[MAX_COMMAND_LEN], edge[MAX_COMMAND_LEN], to[MAX_COMMAND_LEN];
        int nitems = sscanf(line_buf, "%s %s %s %s", type, v, to, edge);
        assert(nitems >= 2);

        if (nitems == 2) {
            if (strcmp(type, "brute-vertex-add") == 0) {
                simple_tic(timer);
                cfpq_brute_vertex_added(graph, grammar, response, v);
                total_time += simple_toc(timer);
            } else if (strcmp(type, "smart-vertex-add") == 0) {
                simple_tic(timer);
                cfpq_smart_vertex_added(graph, grammar, response, v);
                total_time += simple_toc(timer);
            } else if (strcmp(type, "brute-vertex-delete") == 0) {
                simple_tic(timer);
                cfpq_brute_vertex_deleted(graph, grammar, response, v);
                total_time += simple_toc(timer);
            } else if (strcmp(type, "smart-vertex-delete") == 0) {
                simple_tic(timer);
                cfpq_smart_vertex_deleted(graph, grammar, response, v);
                total_time += simple_toc(timer);
            }
        } else if (nitems == 3) {
            if (strcmp(type, "find-path") == 0) {
                GrB_Index v_id = atoll(v);
                GrB_Index to_id = atoll(to);

                bool result = false;

                simple_tic(timer);
                GrB_Matrix_extractElement_BOOL(&result, response->nonterminal_matrices[0], v_id, to_id);
                total_time += simple_toc(timer);
            }
        } else if (nitems == 4) {
            if (strcmp(type, "brute-edge-add") == 0) {
                GraphRepr_InsertEdge(graph, grammar, v, edge, to);
                simple_tic(timer);
                cfpq_brute_edge_added(graph, grammar, response, v, edge, to);
                total_time += simple_toc(timer);
            } else if (strcmp(type, "smart-edge-add") == 0) {
                GraphRepr_InsertEdge(graph, grammar, v, edge, to);
                simple_tic(timer);
                cfpq_smart_edge_added(graph, grammar, response, v, edge, to);
                total_time += simple_toc(timer);
            } else if (strcmp(type, "brute-edge-delete") == 0) {
                GraphRepr_DeleteEdge(graph, grammar, v, edge, to);
                simple_tic(timer);
                cfpq_brute_edge_deleted(graph, grammar, response, v, edge, to);
                total_time += simple_toc(timer);
            } else if (strcmp(type, "smart-edge-delete") == 0) {
                GraphRepr_DeleteEdge(graph, grammar, v, edge, to);
                simple_tic(timer);
                cfpq_smart_edge_deleted(graph, grammar, response, v, edge, to);
                total_time += simple_toc(timer);
            }
        }
    }
    printf("Total time: %lf s\n", total_time);
}