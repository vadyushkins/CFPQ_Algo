#include "algorithms.h"

void cfpq_brute_edge_deleted(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v, const char* edge, const char* to) {
    cfpq_static(graph, grammar, response);

    for (GrB_Index i = 0; i < grammar->nonterminals.count; ++i) {
        char* nonterm = ItemMapper_Map((ItemMapper*) &grammar->nonterminals, i);
        GrB_Index nvals;
        GrB_Matrix_nvals(&nvals, response->nonterminal_matrices[i]);
        printf("%s: %ld\n", nonterm, nvals);
    }
}