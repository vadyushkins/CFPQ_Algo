#include "graph.h"

void GraphRepr_Init(GraphRepr* graph) {
    // Initialize nodes count
    graph->nodes_count = 0;

    // Initialie terminal matrices
    for (GrB_Index i = 0; i < MAX_GRAMMAR_TERMINALS; ++i) {
        GrB_Matrix_new(&graph->terminal_matrices[i], GrB_BOOL, MAX_GRAPH_SIZE, MAX_GRAPH_SIZE);
    }
} 

void GraphRepr_InsertEdge(GraphRepr* graph, const Grammar* grammar, const char* v, const char* edge, const char* to) {
    GrB_Index v_id = atoll(v);
    GrB_Index to_id = atoll(to);
    GrB_Index edge_id = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->terminals, edge);

    assert(v_id != MAX_GRAPH_SIZE && to_id != MAX_GRAPH_SIZE && edge_id != MAX_GRAMMAR_TERMINALS);

    GrB_Matrix_setElement_BOOL(graph->terminal_matrices[edge_id], true, v_id, to_id);

    #ifdef DEBUG
    GrB_Index nvals;
    GrB_Matrix_nvals(&nvals, graph->terminal_matrices[edge_id]);
    printf("Inserted edge: %s %s %s : %ld %ld %ld, nvals= %ld\n", v, edge, to, v_id, edge_id, to_id, nvals);
    #endif

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

    // Initialize input  
    char* buf = NULL;
    size_t buf_size = 0;

    // Read graph
    while (getline(&buf, &buf_size, f) != -1) {
        buf[strcspn(buf, "\n")] = 0;

        char* v = (char*) malloc(MAX_VERTEX_NAME_LEN * sizeof(char));
        char* edge = (char*) malloc(MAX_EDGE_NAME_LEN * sizeof(char));
        char* to = (char*) malloc(MAX_VERTEX_NAME_LEN * sizeof(char));

        int nitems = sscanf(buf, "%s %s %s", v, edge, to);
        assert(nitems == 3);

        GraphRepr_InsertEdge(graph, grammar, v, edge, to);

        free(to);
        free(edge);
        free(v);
    }
    free(buf);
}
