#pragma once

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../deps/GraphBLAS/Include/GraphBLAS.h"
#include "../cfpq_algorithms/algorithms.h"
#include "../graph/graph.h"
#include "../grammar/grammar.h"
#include "../response/response.h"
#include "../mapper/mapper.h"
#include "../config/config.h"
#include "../timer/simple_timer.h"

void interprete_queries(
    GraphRepr* graph, 
    const Grammar* grammar, 
    Response* response, 
    FILE* f
);