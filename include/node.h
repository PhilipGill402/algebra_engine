#ifndef INCLUDE_NODE_H_
#define INCLUDE_NODE_H_

#include <stdlib.h>
#include <stdint.h>
#include "string_t.h"

typedef enum {
    NODE_CONSTANT,
    NODE_VARIABLE,
    NODE_FUNCTION,
    NODE_OP
} NodeType;

typedef union {
    char op;
    double num;
    string_t* id;
} value_t;

typedef struct node_t node_t;

struct node_t {
    struct node_t* left;
    struct node_t* right;
    NodeType type;
    value_t val;
    uint8_t can_be_simplified;
};

void node_free(node_t* node);
node_t* node_clone(node_t* node);
node_t* create_constant(double num);
node_t* create_variable(string_t* var);
node_t* create_function(string_t* func);
node_t* create_op(char op);

#endif // !INCLUDE_NODE_H_
