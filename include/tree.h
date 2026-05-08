#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include "string_t.h"

typedef enum {
    NODE_CONSTANT,
    NODE_VARIABLE,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_EXP
} NodeType;

typedef union {
    char op;
    double num;
    string_t* function;
} value_t;

typedef struct {
    struct node_t* left;
    struct node_t* right;
    NodeType type;
    value_t val;
} node_t;



#endif
