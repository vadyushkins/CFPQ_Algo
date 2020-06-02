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

    // Addition matrix
    GraphRepr addition;
    GraphRepr_Init(&addition);

    for (GrB_Index i = 0; i < graph.nodes.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &addition.nodes, 
            ItemMapper_Map((ItemMapper*) &graph.nodes, i)
        );
    }

    for (GrB_Index i = 0; i < graph.edges.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &addition.edges, 
            ItemMapper_Map((ItemMapper*) &graph.edges, i)
        );
    }

    // Deletion matrix 
    GraphRepr deletion;
    GraphRepr_Init(&deletion);

    for (GrB_Index i = 0; i < graph.nodes.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &deletion.nodes, 
            ItemMapper_Map((ItemMapper*) &graph.nodes, i)
        );
    }

    for (GrB_Index i = 0; i < graph.edges.count; ++i) {
        ItemMapper_Insert(
            (ItemMapper*) &deletion.edges, 
            ItemMapper_Map((ItemMapper*) &graph.edges, i)
        );
    }

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
        interprete_queries_without_time(&graph, &grammar, &response, &addition, &deletion, f);
    } else if (PRINT_TIME == 1){
        double timer[2];
        simple_tic(timer);
        cfpq_static(&graph, &grammar, &response);
        double time_query = simple_toc(timer);
        printf("Initialize time: %lf s\n", time_query);
        
        simple_tic(timer);
        interprete_queries_with_time(&graph, &grammar, &response, &addition, &deletion, f);
        time_query = simple_toc(timer);
        printf("Total time: %lf s\n", time_query);
    } else if (PRINT_TIME == 2) {
        double timer[2];
        simple_tic(timer);
        cfpq_static(&graph, &grammar, &response);
        double time_query = simple_toc(timer);
        printf("Initialize time: %lf s\n", time_query);
        
        simple_tic(timer);
        interprete_queries_without_time(&graph, &grammar, &response, &addition, &deletion, f);
        time_query = simple_toc(timer);
        printf("Total time: %lf s\n", time_query);
    }

    fclose(f);
}