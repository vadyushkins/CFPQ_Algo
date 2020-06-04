#include "response.h"

void Response_Init(Response* response, const Grammar* grammar) {
    response->iteration_count = 0;
    response->nonterminals = grammar->nonterminals;
    response->nonterminals_count = grammar->nonterminals.count;
    for (GrB_Index i = 0; i < response->nonterminals_count; ++i) {
        GrB_Matrix_new(&response->nonterminal_matrices[i], GrB_BOOL, MAX_GRAPH_SIZE, MAX_GRAPH_SIZE);
    }
}

void Response_Print(Response* response) {
    printf("iteration count: %ld\n", response->iteration_count);
    for (GrB_Index i = 0; i < response->nonterminals_count; ++i) {
        printf("i = %ld, nonterm = %s", i, ItemMapper_Map((ItemMapper*) &response->nonterminals, i));
        GxB_print(response->nonterminal_matrices[i], GxB_COMPLETE);
    }
}