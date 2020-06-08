#include "algorithms.h"

void cfpq_static(const Graph* graph, const Grammar* grammar, Response* response) {
    // Initialize response
    for (GrB_Index i = 0; i < response->nonterminals_count; ++i) {
        GrB_Matrix_free(&response->nonterminal_matrices[i]);
        GrB_free(&response->nonterminal_matrices[i]);
        GrB_Matrix_new(&response->nonterminal_matrices[i], GrB_BOOL, MAX_GRAPH_SIZE, MAX_GRAPH_SIZE);
    }
    
    // Initialize matrices
    for (GrB_Index i = 0; i < grammar->terminals.count; ++i) {
        for (GrB_Index j = 0; j < grammar->simple_rules_count; ++j) {
            const SimpleRule* simpleRule = &grammar->simple_rules[j];
            if (simpleRule->r == i) {
                GrB_eWiseAdd(
                    response->nonterminal_matrices[simpleRule->l],
                    GrB_NULL,
                    GrB_LOR,
                    GrB_LOR,
                    response->nonterminal_matrices[simpleRule->l],
                    graph->terminal_matrices[i],
                    GrB_NULL
                );
            }
        }
    }

    response->iteration_count = 0;

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
    printf("Start printing cfpq_static control sums\n");

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

    printf("Finish printing cfpq_static control sums\n");
    #endif

    printf("Iteration count: %ld\n", response->iteration_count);
}