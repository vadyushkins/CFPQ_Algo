#pragma once

#include <assert.h>
#include <stdlib.h>

#include "../deps/GraphBLAS/Include/GraphBLAS.h"
#include "../graph/graph.h"
#include "../grammar/grammar.h"
#include "../response/response.h"
#include "../mapper/mapper.h"
#include "../config/config.h"
#include "../timer/simple_timer.h"

// Standard CFPQ Algorithms

// Returns success status of the algorithm
void cfpq_static(const GraphRepr* graph, const Grammar* grammar, Response* response);

// Brute Dynamic CFPQ Algorithms

void cfpq_brute_vertex_delete(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v);

// Atomic Dynamic CFPQ Operations

void cfpq_vertex_delete(const GraphRepr* graph, const Grammar* grammar, Response* response, const char* v);

void cfpq_edge_added(const GraphRepr* graph, const Grammar* grammar, Response* response);

void cfpq_edge_deleted(const GraphRepr* graph, const Grammar* grammar, Response* response, const GrB_Index v_id, const GrB_Index edge_id, const GrB_Index to_id);
