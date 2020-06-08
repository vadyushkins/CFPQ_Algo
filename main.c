#include <assert.h>

#include "interpreter/interpreter.h"
#include "timer/simple_timer.h"

char GRAPH_INPUT_FILE[MAX_COMMAND_LEN];
char GRAMMAR_INPUT_FILE[MAX_COMMAND_LEN];
char QUERIES_INPUT_FILE[MAX_COMMAND_LEN];

void usage() {
    printf("Usage:\n");
    printf("./main {graph} {grammar} {queries}\n");
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        usage();
    }

    strcpy(GRAPH_INPUT_FILE, argv[1]);
    strcpy(GRAMMAR_INPUT_FILE, argv[2]);
    strcpy(QUERIES_INPUT_FILE, argv[3]);

    // Initialize GraphBLAS
    GrB_init(GrB_NONBLOCKING);

    // Load grammar
    FILE* f = fopen(GRAMMAR_INPUT_FILE, "r");
    assert(f != NULL);

    Grammar grammar;
    Grammar_Init(&grammar);
    Grammar_Load(&grammar, f);
    fclose(f);

    // Load graph
    f = fopen(GRAPH_INPUT_FILE, "r");
    assert(f != NULL);

    GraphRepr graph;
    GraphRepr_Init(&graph);
    GraphRepr_Load(&graph, &grammar, f);
    fclose(f);

    // Initialize response
    Response response;
    Response_Init(&response, &grammar);

    f = fopen(QUERIES_INPUT_FILE, "r");

    // Initialize graph
    cfpq_static(&graph, &grammar, &response);

    #ifdef DEBUG
    // Control sums

    printf("Start printing precalc control sums\n");

    for (GrB_Index i = 0; i < response.nonterminals_count; ++i) {
        char* nonterm = ItemMapper_Map((ItemMapper*) &response.nonterminals, i);
        GrB_Index result = 0;
        GrB_Matrix_nvals(&result, response.nonterminal_matrices[i]);
        printf("%s: %ld\n", nonterm, result);
    }

    for (GrB_Index i = 0; i < grammar.terminals.count; ++i) {
        char* term = ItemMapper_Map((ItemMapper*) &grammar.terminals, i);
        GrB_Index result = 0;
        GrB_Matrix_nvals(&result, graph.terminal_matrices[i]);
        printf("%s: %ld\n", term, result);
    }

    printf("Finish printing precalc control sums\n");
    #endif

    // Interprete queries
    interprete_queries(&graph, &grammar, &response, f);
    fclose(f);

    #ifdef DEBUG
    // Control sums

    printf("Start printing result control sums\n");

    for (GrB_Index i = 0; i < response.nonterminals_count; ++i) {
        char* nonterm = ItemMapper_Map((ItemMapper*) &response.nonterminals, i);
        GrB_Index result = 0;
        GrB_Matrix_nvals(&result, response.nonterminal_matrices[i]);
        printf("%s: %ld\n", nonterm, result);
    }

    for (GrB_Index i = 0; i < grammar.terminals.count; ++i) {
        char* term = ItemMapper_Map((ItemMapper*) &grammar.terminals, i);
        GrB_Index result = 0;
        GrB_Matrix_nvals(&result, graph.terminal_matrices[i]);
        printf("%s: %ld\n", term, result);
    }

    printf("Finish printing result control sums\n");
    #endif

    // Free
    for (GrB_Index i = 0; i < MAX_GRAMMAR_TERMINALS; ++i) {
        GrB_Matrix_free(&graph.terminal_matrices[i]);
        GrB_free(&graph.terminal_matrices[i]);
    }

    for (GrB_Index i = 0; i < response.nonterminals_count; ++i) {
        GrB_Matrix_free(&response.nonterminal_matrices[i]);
        GrB_free(&response.nonterminal_matrices[i]);
    }
}