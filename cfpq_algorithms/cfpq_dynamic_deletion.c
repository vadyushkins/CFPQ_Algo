#include "algorithms.h"

void cfpq_dynamic_deletion(const GraphRepr* graph, const Grammar* grammar, Response* response, const GraphRepr* deletion) {
    for (GrB_Index i = 0; i < deletion->nodes.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &graph->nodes, 
            ItemMapper_Map((ItemMapper*) &deletion->nodes, i)
        );
    }

    for (GrB_Index i = 0; i < deletion->edges.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &graph->edges, 
            ItemMapper_Map((ItemMapper*) &deletion->edges, i)
        );
    }
    
    // Create del matrices
    GrB_Matrix del_matrices[response->nonterminals_count];

    for (GrB_Index i = 0; i < response->nonterminals_count; ++i) {
        GrB_Matrix_new(
            &del_matrices[i], 
            GrB_BOOL, 
            MAX_GRAPH_SIZE, 
            MAX_GRAPH_SIZE
        );
    }

    // Initialize del matrices
    for (GrB_Index i = 0; i < deletion->edges.count; ++i) {
        const char* terminal = deletion->edges.items[i];

        GrB_Index terminal_id = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->terminals, terminal);

        if (terminal_id != grammar->terminals.count) {
            for (GrB_Index j = 0; j < grammar->simple_rules_count; ++j) {
                const SimpleRule* simpleRule = &grammar->simple_rules[j];
                if (simpleRule->r == terminal_id) {
                    GrB_Matrix_dup(
                        &del_matrices[simpleRule->l], 
                        deletion->terminal_matrices[i]
                    );
                }
            }
        }
    }

    response->iteration_count = 0;

    // Algorithm's body
    bool matrices_is_changed = true;
    while(matrices_is_changed) {
        ++response->iteration_count;

        matrices_is_changed = false;

        for (GrB_Index i = 0; i < grammar->complex_rules_count; ++i) {
            GrB_Index nonterm1 = grammar->complex_rules[i].l;
            GrB_Index nonterm2 = grammar->complex_rules[i].r1;
            GrB_Index nonterm3 = grammar->complex_rules[i].r2;

            GrB_Matrix m_old;
            GrB_Matrix_dup(&m_old, del_matrices[nonterm1]);

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

            GrB_Index nvals_new, nvals_old;
            GrB_Matrix_nvals(&nvals_new, del_matrices[nonterm1]);
            GrB_Matrix_nvals(&nvals_old, m_old);
            if (nvals_new != nvals_old) {
                matrices_is_changed = true;
            }

            GrB_Matrix_free(&m_old);
            GrB_free(&m_old);
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
        ++response->iteration_count;

        matrices_is_changed = false;

        for (GrB_Index i = 0; i < grammar->complex_rules_count; ++i) {
            GrB_Index nonterm1 = grammar->complex_rules[i].l;
            GrB_Index nonterm2 = grammar->complex_rules[i].r1;
            GrB_Index nonterm3 = grammar->complex_rules[i].r2;

            GrB_Matrix m_old;
            GrB_Matrix_dup(&m_old, response->nonterminal_matrices[nonterm1]);

            GrB_mxm(
                response->nonterminal_matrices[nonterm1], 
                GrB_NULL, 
                GrB_LOR, 
                GxB_LOR_LAND_BOOL,
                response->nonterminal_matrices[nonterm2], 
                response->nonterminal_matrices[nonterm3], 
                GrB_NULL
            );

            GrB_Index nvals_new, nvals_old;
            GrB_Matrix_nvals(&nvals_new, response->nonterminal_matrices[nonterm1]);
            GrB_Matrix_nvals(&nvals_old, m_old);
            if (nvals_new != nvals_old) {
                matrices_is_changed = true;
            }

            GrB_Matrix_free(&m_old);
            GrB_free(&m_old);
        }
    }
}