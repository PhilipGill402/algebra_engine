#include "node.h"

void node_free(node_t* node) {
    if (!node) return;

    node_free(node->left);
    node_free(node->right);

    if (node->type == NODE_FUNCTION || node->type == NODE_VARIABLE)
        string_free(node->val.id);

    free(node);
}

node_t* node_clone(node_t* node) {
    if (!node) return NULL; 

    node_t* clone = malloc(sizeof(node_t));
    clone->left = node_clone(node->left);
    clone->right = node_clone(node->right);
    clone->type = node->type;

    if (clone->type == NODE_FUNCTION || clone->type == NODE_VARIABLE)
        clone->val.id = string_clone(node->val.id);
    else
        clone->val = node->val;

    clone->can_be_simplified = node->can_be_simplified;

    return clone;
}

node_t* create_constant(double num) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_CONSTANT;
    node->val.num = num;
    node->can_be_simplified = 1;

    return node;
}

node_t* create_variable(string_t* var) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_VARIABLE;
    node->val.id = var;
    node->can_be_simplified = 1;

    return node;
}

node_t* create_function(string_t* func) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_FUNCTION;
    node->val.id = func;
    node->can_be_simplified = 1;

    return node;
}

node_t* create_op(char op) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_OP;
    node->val.op = op;
    node->can_be_simplified = 1;

    return node;
}


