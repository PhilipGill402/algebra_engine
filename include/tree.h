#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <stdint.h>
#include "string_t.h"
#include "parser.h"
#include "lexer.h"

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

typedef struct {
    string_t name;
    node_t* node;
} entry_t;


uint8_t can_be_simplified(node_t* node);
node_t* create_tree(parser_t* parser);
void print_inorder_tree(node_t* node);
void node_free(node_t* node);
node_t* node_clone(node_t* node);
//void simplify_tree(node_t* root);

#endif
