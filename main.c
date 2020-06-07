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
    Grammar_Load(&grammar, f);
    fclose(f);

    // Load graph
    f = fopen(GRAPH_INPUT_FILE, "r");
    assert(f != NULL);

    GraphRepr graph;
    GraphRepr_Load(&graph, &grammar, f);
    fclose(f);

    // Initialize response
    Response response;
    Response_Init(&response, &grammar);

    f = fopen(QUERIES_INPUT_FILE, "r");

    // Initialize graph
    cfpq_static(&graph, &grammar, &response);

    // Interprete queries
    interprete_queries(&graph, &grammar, &response, f);
    
    fclose(f);
}