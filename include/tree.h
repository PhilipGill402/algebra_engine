#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include "string_t.h"
#include "parser.h"

typedef enum {
    NODE_CONSTANT,
    NODE_VARIABLE,
    NODE_FUNCTION,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_EXP
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
};

typedef struct {
    string_t name;
    node_t* node;
} entry_t;

node_t* create_tree(parser_t* parser);
void print_tree(node_t* node);

#endif
