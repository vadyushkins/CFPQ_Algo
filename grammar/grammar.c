#include "grammar.h"

void Grammar_Init(Grammar* grammar) {
    grammar->complex_rules_count = 0;
    grammar->simple_rules_count = 0;

    ItemMapper_Init((ItemMapper*) &grammar->nonterminals);
    ItemMapper_Init((ItemMapper*) &grammar->terminals);
}

int Grammar_Load(Grammar* grammar, FILE* f) {
    Grammar_Init(grammar);

    char* buf;
    size_t buf_size = 0;
    size_t ptr = 0;

    char nonterm[MAX_ITEM_NAME_LEN];
    char term[MAX_ITEM_NAME_LEN];

    getline(&buf, &buf_size, f);
    while (sscanf(buf + ptr, "%s", nonterm) == 1) {
        ItemMapper_Insert((ItemMapper*) &grammar->nonterminals, nonterm);
        ptr += strlen(nonterm) + 1;
    }
    ptr = 0;

    getline(&buf, &buf_size, f);
    while (sscanf(buf + ptr, "%s", term) == 1) {
        ItemMapper_Insert((ItemMapper*) &grammar->terminals, term);
        ptr += strlen(term) + 1;
    }
    ptr = 0;

    while (getline(&buf, &buf_size, f) != -1) {
        buf[strcspn(buf, "\n")] = 0;

        char l[MAX_ITEM_NAME_LEN], r1[MAX_ITEM_NAME_LEN], r2[MAX_ITEM_NAME_LEN];
        int nitems = sscanf(buf, "%s %s %s", l, r1, r2);

        if (nitems == 2) {
            int gr_l = ItemMapper_Insert((ItemMapper*) &grammar->nonterminals, l);
            int gr_r = ItemMapper_Insert((ItemMapper*) &grammar->terminals, r1);

            Grammar_AddSimpleRule(grammar, gr_l, gr_r);
        } else if (nitems == 3) {
            int gr_l = ItemMapper_Insert((ItemMapper*) &grammar->nonterminals, l);
            int gr_r1 = ItemMapper_Insert((ItemMapper*) &grammar->nonterminals, r1);
            int gr_r2 = ItemMapper_Insert((ItemMapper*) &grammar->nonterminals, r2);

            Grammar_AddComplexRule(grammar, gr_l, gr_r1, gr_r2);
        } else {
            return GRAMMAR_LOAD_ERROR;
        }
    }
    return GRAMMAR_LOAD_SUCCESS;
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
