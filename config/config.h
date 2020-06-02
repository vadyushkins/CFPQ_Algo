#pragma once

#include <stdint.h>

#include "../deps/GraphBLAS/Include/GraphBLAS.h"

#define MAX_GRAPH_SIZE 1000000
#define MAX_GRAMMAR_SIZE 2000

#define MAX_VERTEX_NAME_LEN 10
#define MAX_EDGE_NAME_LEN 255
#define MAX_ITEM_NAME_LEN 10

#define MAX_COMMAND_LEN 255

#define MAX_GRAMMAR_TERMINALS 1000
#define MAX_GRAMMAR_NONTERMINALS 1000

#define GRAMMAR_LOAD_ERROR 0
#define GRAMMAR_LOAD_SUCCESS 1

#define ITEM_NOT_EXIST 0
#define ITEM_EXIST 1