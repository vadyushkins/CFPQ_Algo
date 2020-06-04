#pragma once

#include <assert.h>
#include <stdio.h>

#include "../config/config.h"
#include "../grammar/grammar.h"
#include "../mapper/mapper.h"

typedef struct {
    GrB_Matrix terminal_matrices[MAX_GRAMMAR_TERMINALS];

    TermMapper terminals;
} GraphRepr;

void GraphRepr_Load(const GraphRepr* graph, FILE* f);
void GraphRepr_InsertEdge(const GraphRepr* graph, const char* v, const char* edge, const char* to);
void GraphRepr_DeleteEdge(const GraphRepr* graph, const char* v, const char* edge, const char* to);
