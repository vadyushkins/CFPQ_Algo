#pragma once

#include <assert.h>
#include <stdio.h>

#include "../config/config.h"
#include "../grammar/grammar.h"
#include "../mapper/mapper.h"

typedef struct {
    GrB_Index count;
    char items[MAX_GRAPH_SIZE][MAX_ITEM_NAME_LEN];
} EntryMapper;

typedef struct {
    GrB_Matrix terminal_matrices[MAX_GRAMMAR_TERMINALS];

    EntryMapper edges;
    EntryMapper nodes;
} GraphRepr;

void GraphRepr_Init(const GraphRepr* graph);

void GraphRepr_InsertEdge(const GraphRepr* graph, const char* v, const char* edge, const char* to);
void GraphRepr_DeleteEdge(const GraphRepr* graph, const char* v, const char* edge, const char* to);

void GraphRepr_Load(const GraphRepr* graph, FILE* f);
