#pragma once

#include <assert.h>

#include "../config/config.h"
#include "../mapper/mapper.h"
#include "../grammar/grammar.h"

typedef struct {
    GrB_Index iteration_count;

    GrB_Matrix nonterminal_matrices[MAX_GRAMMAR_NONTERMINALS];

    NontermMapper nonterminals;
    
    GrB_Index nonterminals_count;
} Response;

void Response_Init(Response* response, const Grammar* grammar);
void Response_Print(Response* response);
