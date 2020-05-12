#pragma once

#include <stdio.h>
#include <assert.h>

#include "../config/config.h"
#include "../mapper/mapper.h"

typedef struct {
    GrB_Index count;
    char items[MAX_GRAMMAR_NONTERMINALS][MAX_ITEM_NAME_LEN];
} NontermMapper;

typedef struct {
    GrB_Index count;
    char items[MAX_GRAMMAR_TERMINALS][MAX_ITEM_NAME_LEN];
} TermMapper;

typedef struct {
    GrB_Index l;
    GrB_Index r1;
    GrB_Index r2;
} ComplexRule;

typedef struct {
    GrB_Index l;
    GrB_Index r;
} SimpleRule;

typedef struct {
    ComplexRule complex_rules[MAX_GRAMMAR_SIZE];
    int complex_rules_count;

    SimpleRule simple_rules[MAX_GRAMMAR_SIZE];
    int simple_rules_count;

    NontermMapper nonterminals;
    TermMapper terminals;
} Grammar;

int Grammar_Load(Grammar* grammar, FILE* f);
void Grammar_Init(Grammar* grammar);

void Grammar_AddSimpleRule(Grammar* grammar, GrB_Index l, GrB_Index r);
void Grammar_AddComplexRule(Grammar* grammar, GrB_Index l, GrB_Index r1, GrB_Index r2);
