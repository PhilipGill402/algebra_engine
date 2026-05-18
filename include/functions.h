#ifndef INCLUDE_FUNCTIONS_H_
#define INCLUDE_FUNCTIONS_H_

#include <math.h>
#include "vector.h"
#include "string_t.h"
#include "node.h"

typedef struct {
    char* name;
    double (*func)(double x);
    node_t* (*diff)(node_t* self);
} function_t;

void functions_init();

extern vector_t functions;

#endif // !INCLUDE_FUNCTIONS_H_


