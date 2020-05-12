#include "interpreter.h"

void interprete_queries(
    const GraphRepr* graph, 
    const Grammar* grammar, 
    Response* response, 
    FILE* f,
    int print_time
) {
    char* line_buf;
    size_t buf_size = 0;

    while (getline(&line_buf, &buf_size, f) != -1) {
        line_buf[strcspn(line_buf, "\n")] = 0;

        char type[MAX_ITEM_NAME_LEN];
        char v[MAX_ITEM_NAME_LEN], edge[MAX_ITEM_NAME_LEN], to[MAX_ITEM_NAME_LEN];
        int nitems = sscanf(line_buf, "%s %s %s %s", type, v, to, edge);
        assert(nitems >= 2);

        if (nitems == 2) {
            if (strcmp(type, "brute-vertex-add") == 0) {
                double timer[2];
                simple_tic(timer);
                cfpq_brute_vertex_add(graph, grammar, response, v);
                double time_query = simple_toc(timer);
                if (print_time == 1) {
                    printf("%s: %s, time= %lf s\n", type, v, time_query);
                }
            } else if (strcmp(type, "smart-vertex-add") == 0) {
                double timer[2];
                simple_tic(timer);
                cfpq_vertex_add(graph, grammar, response, v);
                double time_query = simple_toc(timer);
                if (print_time == 1) {
                    printf("%s: %s, time= %lf s\n", type, v, time_query);
                }
            } else if (strcmp(type, "brute-vertex-delete") == 0) {
                double timer[2];
                simple_tic(timer);
                cfpq_brute_vertex_delete(graph, grammar, response, v);
                double time_query = simple_toc(timer);
                if (print_time == 1) {
                    printf("%s: %s, time= %lf s\n", type, v, time_query);
                }
            } else if (strcmp(type, "smart-vertex-delete") == 0) {
                double timer[2];
                simple_tic(timer);
                cfpq_vertex_delete(graph, grammar, response, v);
                double time_query = simple_toc(timer);
                if (print_time == 1) {
                    printf("%s: %s, time= %lf s\n", type, v, time_query);
                }
            }
        } else if (nitems == 3) {
            if (strcmp(type, "find-path") == 0) {
                GrB_Index v_id = ItemMapper_GetPlaceIndex((ItemMapper*) &graph->nodes, v);
                GrB_Index to_id = ItemMapper_GetPlaceIndex((ItemMapper*) &graph->nodes, to);

                bool result = false;

                #ifdef DEBUG
                    printf("%s: v = %s %ld\n", __func__, v, v_id);
                    printf("%s: to = %s %ld\n", __func__, to, to_id);
                    printf("%s: graph->nodes.count= %ld\n", __func__, graph->nodes.count);
                    printf("%s: graph->nodes.count= %ld\n", __func__, graph->edges.count);
                #endif

                double timer[2];
                simple_tic(timer);
                GrB_Matrix_extractElement_BOOL(&result, response->nonterminal_matrices[0], v_id, to_id);
                double time_query = simple_toc(timer);
                if (print_time == 1) {
                    printf("%s: %ld %ld %s, time= %lf s\n", type, v_id, to_id, result?"exist":"not exist", time_query);
                } else {
                    printf("%s: %ld %ld %s\n", type, v_id, to_id, result?"exist":"not exist");
                }
            }
        } else if (nitems == 4) {
            if (strcmp(type, "brute-edge-add") == 0) {
                double timer[2];
                simple_tic(timer);
                cfpq_brute_edge_add(graph, grammar, response, v, edge, to);
                double time_query = simple_toc(timer);
                if (print_time == 1) {
                    printf("%s: %s %s %s, time= %lf s\n", type, v, edge, to, time_query);
                }
            } else if (strcmp(type, "smart-edge-add") == 0) {
                double timer[2];
                simple_tic(timer);
                cfpq_edge_add(graph, grammar, response, v, edge, to);
                double time_query = simple_toc(timer);
                if (print_time == 1) {
                    printf("%s: %s %s %s, time= %lf s\n", type, v, edge, to, time_query);
                }
            } else if (strcmp(type, "brute-edge-delete") == 0) {
                double timer[2];
                simple_tic(timer);
                cfpq_brute_edge_delete(graph, grammar, response, v, edge, to);
                double time_query = simple_toc(timer);
                if (print_time == 1) {
                    printf("%s: %s %s %s, time= %lf s\n", type, v, edge, to, time_query);
                }
            } else if (strcmp(type, "smart-edge-delete") == 0) {
                double timer[2];
                simple_tic(timer);
                cfpq_edge_delete(graph, grammar, response, v, edge, to);
                double time_query = simple_toc(timer);
                if (print_time == 1) {
                    printf("%s: %s %s %s, time= %lf s\n", type, v, edge, to, time_query);
                }
            }
        }
    }
}