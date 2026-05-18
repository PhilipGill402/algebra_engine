#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <stdint.h>
#include "string_t.h"
#include "parser.h"
#include "lexer.h"
#include "node.h"

typedef struct {
    string_t name;
    node_t* node;
} entry_t;


uint8_t can_be_simplified(node_t* node);
node_t* create_tree(parser_t* parser);
void print_inorder_tree(node_t* node);

//void simplify_tree(node_t* root);

#endif
