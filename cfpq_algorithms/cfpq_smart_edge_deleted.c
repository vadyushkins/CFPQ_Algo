#include "algorithms.h"

void cfpq_smart_edge_deleted(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v, const char* edge, const char* to) {    // Create del matrices
    GrB_Index v_id = atoll(v);
    GrB_Index to_id = atoll(to);
    GrB_Index edge_id = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->terminals, edge);

    GrB_Matrix del_matrices[response->nonterminals_count];

    for (GrB_Index i = 0; i < response->nonterminals_count; ++i) {
        GrB_Matrix_new(
            &del_matrices[i], 
            GrB_BOOL, 
            MAX_GRAPH_SIZE, 
            MAX_GRAPH_SIZE
        );
    }

    // Initialize del_matrices
    for (GrB_Index j = 0; j < grammar->simple_rules_count; ++j) {
        const SimpleRule* simpleRule = &grammar->simple_rules[j];
        if (simpleRule->r == edge_id) {
            GrB_Matrix_setElement(del_matrices[simpleRule->l], v_id, to_id, true);
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
            GrB_Matrix_nvals(&nvals_old, del_matrices[nonterm1]);

            GrB_mxm(
                del_matrices[nonterm1], 
                GrB_NULL, 
                GrB_LOR, 
                GxB_LOR_LAND_BOOL,
                response->nonterminal_matrices[nonterm2], 
                del_matrices[nonterm3], 
                GrB_NULL
            );

            GrB_mxm(
                del_matrices[nonterm1], 
                GrB_NULL, 
                GrB_LOR, 
                GxB_LOR_LAND_BOOL,
                del_matrices[nonterm2], 
                response->nonterminal_matrices[nonterm3], 
                GrB_NULL
            );

            response->iteration_count += 2;

            GrB_Index nvals_new;
            GrB_Matrix_nvals(&nvals_new, del_matrices[nonterm1]);
            if (nvals_new != nvals_old) {
                matrices_is_changed = true;
            } 
        }
    }

    for (GrB_Index i = 0; i < grammar->nonterminals.count; ++i) {
        GrB_eWiseAdd(
            response->nonterminal_matrices[i], 
            GrB_NULL, 
            GrB_LOR, 
            GrB_MINUS_BOOL, 
            response->nonterminal_matrices[i], 
            del_matrices[i], 
            GrB_NULL
        );
    }

    matrices_is_changed = true;
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