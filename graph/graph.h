#pragma once

#include <assert.h>
#include <stdio.h>

#include "../config/config.h"
#include "../grammar/grammar.h"
#include "../mapper/mapper.h"

typedef struct {
    GrB_Matrix terminal_matrices[MAX_GRAMMAR_TERMINALS];

    GrB_Index nodes_count;
} Graph;

void Graph_Init(Graph* graph);
void Graph_Load(Graph* graph, const Grammar* grammar, FILE* f);
void Graph_InsertEdge(Graph* graph, const Grammar* grammar, const char* v, const char* edge, const char* to);
void Graph_DeleteEdge(Graph* graph, const Grammar* grammar, const char* v, const char* edge, const char* to);
