#include "grammar.h"

void Grammar_Init(Grammar* grammar) {
    // Initialize grammar
    grammar->complex_rules_count = 0;
    grammar->simple_rules_count = 0;

    ItemMapper_Init((ItemMapper*) &grammar->nonterminals);
    ItemMapper_Init((ItemMapper*) &grammar->terminals);
}

void Grammar_Load(Grammar* grammar, FILE* f) {
    // Initialize input
    char* buf = NULL;
    size_t buf_size = 0;
    size_t ptr = 0;

    // Read nonterminals
    char* nonterm = (char*) malloc(MAX_EDGE_NAME_LEN * sizeof(char));

    if (getline(&buf, &buf_size, f) != -1) {
        while (sscanf(buf + ptr, "%s", nonterm) == 1) {
            ItemMapper_Insert((ItemMapper*) &grammar->nonterminals, nonterm);
            ptr += strlen(nonterm) + 1;
        }
        ptr = 0;
        free(nonterm);
    } else {
        free(nonterm);
        assert(0 && "Can not read nonterminals in grammar\n");
    }

    // Read terminals
    char* term = (char*) malloc(MAX_EDGE_NAME_LEN * sizeof(char));

    if (getline(&buf, &buf_size, f) != -1) {
        while (sscanf(buf + ptr, "%s", term) == 1) {
            ItemMapper_Insert((ItemMapper*) &grammar->terminals, term);
            ptr += strlen(term) + 1;
        }
        ptr = 0;
        free(term);
    } else {
        free(term);
        assert(0 && "Can not read terminals in grammar\n");
    }

    while (getline(&buf, &buf_size, f) != -1) {
        buf[strcspn(buf, "\n")] = 0;

        char* l = (char*) malloc(MAX_EDGE_NAME_LEN * sizeof(char));
        char* r1 = (char*) malloc(MAX_EDGE_NAME_LEN * sizeof(char));
        char* r2 = (char*) malloc(MAX_EDGE_NAME_LEN * sizeof(char));

        int nitems = sscanf(buf, "%s %s %s", l, r1, r2);

        if (nitems == 2) {
            GrB_Index gr_l = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->nonterminals, l);
            GrB_Index gr_r = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->terminals, r1);

            Grammar_AddSimpleRule(grammar, gr_l, gr_r);
            #ifdef DEBUG
            printf("Added SimpleRule: %s -> %s : %ld -> %ld\n", l, r1, gr_l, gr_r);
            #endif
        } else if (nitems == 3) {
            GrB_Index gr_l = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->nonterminals, l);
            GrB_Index gr_r1 = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->nonterminals, r1);
            GrB_Index gr_r2 = ItemMapper_GetPlaceIndex((ItemMapper*) &grammar->nonterminals, r2);

            Grammar_AddComplexRule(grammar, gr_l, gr_r1, gr_r2);
            #ifdef DEBUG
            printf("Added ComplexRule: %s -> %s %s : %ld -> %ld %ld\n", l, r1, r2, gr_l, gr_r1, gr_r2);
            #endif
        }

        free(r2);
        free(r1);
        free(l);
    }
    #ifdef DEBUG
    printf("SimpleRules count: %ld\n", grammar->simple_rules_count);
    printf("ComplexRules count: %ld\n", grammar->complex_rules_count);
    #endif
    free(buf);
}

void Grammar_AddSimpleRule(Grammar* grammar, GrB_Index l, GrB_Index r) {
    assert(grammar->simple_rules_count != MAX_GRAMMAR_SIZE);
    SimpleRule newSimpleRule = {.l = l, .r = r};
    grammar->simple_rules[grammar->simple_rules_count] = newSimpleRule;
    grammar->simple_rules_count++;
}

void Grammar_AddComplexRule(Grammar* grammar, GrB_Index l, GrB_Index r1, GrB_Index r2) {
    assert(grammar->complex_rules_count != MAX_GRAMMAR_SIZE);
    ComplexRule newComplexRule = {.l = l, .r1 = r1, .r2 = r2};
    grammar->complex_rules[grammar->complex_rules_count] = newComplexRule;
    grammar->complex_rules_count++;
}
