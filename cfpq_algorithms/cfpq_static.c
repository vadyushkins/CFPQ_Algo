#include "algorithms.h"

void cfpq_static(const GraphRepr* graph, const Grammar* grammar, Response* response) {
    // Initialize response
    for (GrB_Index i = 0; i < response->nonterminals_count; ++i) {
        GrB_Matrix_clear(&response->nonterminal_matrices[i]);
    }
    
    // Initialize matrices
    for (GrB_Index i = 0; i < grammar->terminals.count; ++i) {
        const char* terminal = grammar->terminals.items[i];

        GrB_Index terminal_id = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->terminals, terminal);

        if (terminal_id != grammar->terminals.count) {
            for (GrB_Index j = 0; j < grammar->simple_rules_count; ++j) {
                const SimpleRule* simpleRule = &grammar->simple_rules[j];
                if (simpleRule->r == terminal_id) {
                    GrB_Matrix_dup(
                        response->nonterminal_matrices[simpleRule->l],
                        graph->terminal_matrices[i]
                    );
                }
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

    printf("Iteration count: %ld\n", response->iteration_count);
}