#include "interpreter.h"

void interprete_queries(
    Graph* graph, 
    const Grammar* grammar, 
    Response* response,
    FILE* f
) {
    // Initialize timer
    double total_time = 0;
    double timer[2] = {0, 0};

    // Initialize input
    char* buf = NULL;
    size_t buf_size = 0;

    while (getline(&buf, &buf_size, f) != -1) {
        buf[strcspn(buf, "\n")] = 0;

        char* type = (char*) malloc(MAX_COMMAND_LEN * sizeof(char));
        char* v = (char*) malloc(MAX_COMMAND_LEN * sizeof(char));
        char* edge = (char*) malloc(MAX_COMMAND_LEN * sizeof(char));
        char* to = (char*) malloc(MAX_COMMAND_LEN * sizeof(char));
        int nitems = sscanf(buf, "%s %s %s %s", type, v, to, edge);
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
            }
        } else if (nitems == 3) {
            if (strcmp(type, "find-path") == 0) {
                GrB_Index v_id = atoll(v);
                GrB_Index to_id = atoll(to);

                bool result = false;

                simple_tic(timer);
                GrB_Matrix_extractElement_BOOL(&result, response->nonterminal_matrices[0], v_id, to_id);
                total_time += simple_toc(timer);

                if (result == true) {
                    printf("path %s %s exists\n", v, to);
                } else {
                    printf("path %s %s not exists\n", v, to);
                }
            }
        } else if (nitems == 4) {
            if (strcmp(type, "brute-edge-add") == 0) {
                Graph_InsertEdge(graph, grammar, v, edge, to);
                simple_tic(timer);
                cfpq_brute_edge_added(graph, grammar, response, v, edge, to);
                total_time += simple_toc(timer);
            } else if (strcmp(type, "smart-edge-add") == 0) {
                Graph_InsertEdge(graph, grammar, v, edge, to);
                simple_tic(timer);
                cfpq_smart_edge_added(graph, grammar, response, v, edge, to);
                total_time += simple_toc(timer);
            }
        }

        free(to);
        free(edge);
        free(v);
        free(type);
    }
    free(buf);
    printf("Total time: %lf s\n", total_time);
}