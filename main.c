#include <assert.h>

#include "interpreter/interpreter.h"
#include "timer/simple_timer.h"

char GRAPH_INPUT_FILE[MAX_COMMAND_LEN];
char GRAMMAR_INPUT_FILE[MAX_COMMAND_LEN];
char QUERIES_INPUT_FILE[MAX_COMMAND_LEN];
int PRINT_TIME = 0;

void usage() {
    printf("Usage:\n");
    printf("./main {graph} {grammar} {queries} --no-time/--with-time/--total-time\n");
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        usage();
    }

    strcpy(GRAPH_INPUT_FILE, argv[1]);
    strcpy(GRAMMAR_INPUT_FILE, argv[2]);
    strcpy(QUERIES_INPUT_FILE, argv[3]);
    if (strcmp(argv[4], "--no-time") == 0) {
        PRINT_TIME = 0;
    } else if (strcmp(argv[4], "--with-time") == 0) {
        PRINT_TIME = 1;
    } else if (strcmp(argv[4], "--total-time") == 0) {
        PRINT_TIME = 2;
    } else {
        usage();
    }
    
    // Initialize GraphBLAS
    GrB_init(GrB_NONBLOCKING);

    // Load graph
    FILE* f = fopen(GRAPH_INPUT_FILE, "r");
    assert(f != NULL);

    GraphRepr graph;
    GraphRepr_Load(&graph, f);
    fclose(f);

    // Load grammar
    f = fopen(GRAMMAR_INPUT_FILE, "r");
    assert(f != NULL);

    Grammar grammar;
    Grammar_Load(&grammar, f);
    fclose(f);

    // Start algorithm
    Response response;

    f = fopen(QUERIES_INPUT_FILE, "r");

    if (PRINT_TIME == 0) {
        cfpq_static(&graph, &grammar, &response);
        interprete_queries_without_time(&graph, &grammar, &response, f);
    } else if (PRINT_TIME == 1){
        double timer[2];
        simple_tic(timer);
        cfpq_static(&graph, &grammar, &response);
        double time_query = simple_toc(timer);
        printf("Initialize time: %lf s\n", time_query);
        
        simple_tic(timer);
        interprete_queries_with_time(&graph, &grammar, &response, f);
        time_query = simple_toc(timer);
    } else if (PRINT_TIME == 2) {
        double timer[2];
        simple_tic(timer);
        cfpq_static(&graph, &grammar, &response);
        double time_query = simple_toc(timer);
        printf("Initialize time: %lf s\n", time_query);
        
        simple_tic(timer);
        interprete_queries_without_time(&graph, &grammar, &response, f);
        time_query = simple_toc(timer);
    }

    fclose(f);
}