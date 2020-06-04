#include "graph.h"

void GraphRepr_InsertEdge(GraphRepr* graph, const Grammar* grammar, const char* v, const char* edge, const char* to) {
    GrB_Index v_id = atoll(v);
    GrB_Index to_id = atoll(to);
    GrB_Index edge_id = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->terminals, edge);

    assert(v_id != MAX_GRAPH_SIZE && to_id != MAX_GRAPH_SIZE && edge_id != MAX_GRAMMAR_TERMINALS);

    GrB_Matrix_setElement_BOOL(graph->terminal_matrices[edge_id], true, v_id, to_id);

    if (v_id > graph->nodes_count) {
        graph->nodes_count = v_id;
    }

    if (to_id > graph->nodes_count) {
        graph->nodes_count = to_id;
    }
}

void GraphRepr_DeleteEdge(GraphRepr* graph, const Grammar* grammar, const char* v, const char* edge, const char* to) {
    GrB_Index v_id = atoll(v);
    GrB_Index to_id = atoll(to);
    GrB_Index edge_id = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->terminals, edge);

    assert(v_id != MAX_GRAPH_SIZE && to_id != MAX_GRAPH_SIZE && edge_id != MAX_GRAMMAR_TERMINALS);

    GrB_Matrix_setElement_BOOL(graph->terminal_matrices[edge_id], false, v_id, to_id);
}

void GraphRepr_Load(GraphRepr* graph, const Grammar* grammar, FILE* f) {
    // Initialize nodes count
    graph->nodes_count = 0;

    // Initialie terminal matrices
    for (GrB_Index i = 0; i < MAX_GRAMMAR_TERMINALS; ++i) {
        GrB_Matrix_new(&graph->terminal_matrices[i], GrB_BOOL, MAX_GRAPH_SIZE, MAX_GRAPH_SIZE);
    }

    // Initialize input  
    char* line_buf;
    size_t buf_size = 0;

    // Read graph
    while (getline(&line_buf, &buf_size, f) != -1) {
        line_buf[strcspn(line_buf, "\n")] = 0;

        char v[MAX_VERTEX_NAME_LEN], edge[MAX_EDGE_NAME_LEN], to[MAX_VERTEX_NAME_LEN];
        int nitems = sscanf(line_buf, "%s %s %s", v, edge, to);
        assert(nitems == 3);

        GraphRepr_InsertEdge(graph, grammar, v, edge, to);
    }
}
