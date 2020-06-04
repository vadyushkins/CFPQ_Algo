#pragma once

#include <assert.h>
#include <stdio.h>

#include "../config/config.h"
#include "../grammar/grammar.h"
#include "../mapper/mapper.h"

typedef struct {
    GrB_Matrix terminal_matrices[MAX_GRAMMAR_TERMINALS];

    GrB_Index nodes_count;
} GraphRepr;

void GraphRepr_Load(GraphRepr* graph, const Grammar* grammar, FILE* f);
void GraphRepr_InsertEdge(GraphRepr* graph, const Grammar* grammar, const char* v, const char* edge, const char* to);
void GraphRepr_DeleteEdge(GraphRepr* graph, const Grammar* grammar, const char* v, const char* edge, const char* to);
