#include "algorithms.h"

void cfpq_dynamic(const GraphRepr* graph, const Grammar* grammar, Response* response, const GraphRepr* addition, const GraphRepr* deletion) {
    cfpq_dynamic_addition(graph, grammar, response, addition);
    cfpq_dynamic_deletion(graph, grammar, response, deletion);
}