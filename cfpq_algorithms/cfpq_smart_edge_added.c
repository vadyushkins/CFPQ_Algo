#include "algorithms.h"

#include "algorithms.h"

void cfpq_smart_edge_added(const Graph* graph, const Grammar* grammar, Response* response, const char* v, const char* edge, const char* to) {    
    response->iteration_count = 0;

    // Initialize matrices
    GrB_Index edge_id = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->terminals, edge);
    for (GrB_Index i = 0; i < grammar->simple_rules_count; ++i) {
        const SimpleRule* simpleRule = &grammar->simple_rules[i];
        if (simpleRule->r == edge_id) {
            GrB_eWiseAdd(
                response->nonterminal_matrices[simpleRule->l],
                GrB_NULL,
                GrB_LOR,
                GrB_LOR,
                response->nonterminal_matrices[simpleRule->l],
                graph->terminal_matrices[edge_id],
                GrB_NULL
            );
        }
    }

    #ifdef DEBUG
    printf("Start printing cfpq_smart_edge_added control sums before algorithm\n");

    for (GrB_Index i = 0; i < response->nonterminals_count; ++i) {
        char* nonterm = ItemMapper_Map((ItemMapper*) &response->nonterminals, i);
        GrB_Index result = 0;
        GrB_Matrix_nvals(&result, response->nonterminal_matrices[i]);
        printf("%s: %ld\n", nonterm, result);
    }

    for (GrB_Index i = 0; i < grammar->terminals.count; ++i) {
        char* term = ItemMapper_Map((ItemMapper*) &grammar->terminals, i);
        GrB_Index result = 0;
        GrB_Matrix_nvals(&result, graph->terminal_matrices[i]);
        printf("%s: %ld\n", term, result);
    }

    printf("Finish printing cfpq_smart_edge_added control sums before algorithm\n");
    #endif

    // Algorithm's body
    bool matrices_is_changed = true;
    while(matrices_is_changed) {

        matrices_is_changed = false;

        for (GrB_Index i = 0; i < grammar->complex_rules_count; ++i) {
            GrB_Index nonterm1 = grammar->complex_rules[i].l;
            GrB_Index nonterm2 = grammar->complex_rules[i].r1;
            GrB_Index nonterm3 = grammar->complex_rules[i].r2;

            GrB_Index nvals_old;
            GrB_Matrix_nvals(&nvals_old, response->nonterminal_matrices[nonterm1]);

            GrB_mxm(
                response->nonterminal_matrices[nonterm1], 
                GrB_NULL, 
                GrB_LOR, 
                GxB_LOR_LAND_BOOL,
                response->nonterminal_matrices[nonterm2], 
                response->nonterminal_matrices[nonterm3], 
                GrB_NULL
            );

            response->iteration_count += 1;

            GrB_Index nvals_new;
            GrB_Matrix_nvals(&nvals_new, response->nonterminal_matrices[nonterm1]);
            if (nvals_new != nvals_old) {
                matrices_is_changed = true;
            }
        }
    }

    #ifdef DEBUG
    printf("Start printing cfpq_smart_edge_added control sums after algorithm\n");

    for (GrB_Index i = 0; i < response->nonterminals_count; ++i) {
        char* nonterm = ItemMapper_Map((ItemMapper*) &response->nonterminals, i);
        GrB_Index result = 0;
        GrB_Matrix_nvals(&result, response->nonterminal_matrices[i]);
        printf("%s: %ld\n", nonterm, result);
    }

    for (GrB_Index i = 0; i < grammar->terminals.count; ++i) {
        char* term = ItemMapper_Map((ItemMapper*) &grammar->terminals, i);
        GrB_Index result = 0;
        GrB_Matrix_nvals(&result, graph->terminal_matrices[i]);
        printf("%s: %ld\n", term, result);
    }

    printf("Finish printing cfpq_smart_edge_added control sums after algorithm\n");
    #endif

    printf("Iteration count: %ld\n", response->iteration_count);
}