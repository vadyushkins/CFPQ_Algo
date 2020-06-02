#pragma once

#include <assert.h>
#include <stdlib.h>

#include "../deps/GraphBLAS/Include/GraphBLAS.h"
#include "../graph/graph.h"
#include "../grammar/grammar.h"
#include "../response/response.h"
#include "../mapper/mapper.h"
#include "../config/config.h"

// Standard CFPQ Algorithms

// Returns success status of the algorithm
void cfpq_static(const GraphRepr* graph, const Grammar* grammar, Response* response);

// Brute Dynamic CFPQ Algorithms

void cfpq_brute_vertex_delete(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v);

// Smarter Dynamic CFPQ Algorithms

void cfpq_dynamic_addition(const GraphRepr* graph, const Grammar* grammar, Response* response, const GraphRepr* addition);

void cfpq_dynamic_deletion(const GraphRepr* graph, const Grammar* grammar, Response* response, const GraphRepr* deletion);

void cfpq_dynamic(const GraphRepr* graph, const Grammar* grammar, Response* response, const GraphRepr* addition, const GraphRepr* deletion);

// Atomic Dynamic CFPQ Operations

void cfpq_vertex_delete(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v);
