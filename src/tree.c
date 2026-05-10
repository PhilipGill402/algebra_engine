#include "tree.h"

node_t* create_constant(token_t token) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_CONSTANT;
    node->val.num = token.val.num;

    return node;
}

node_t* create_variable(token_t token) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_VARIABLE;
    node->val.id = string_clone(&token.val.name);

    return node;
}

node_t* create_op(token_t token) {
    NodeType type;

    switch(token.val.op) {
        case '+':
            type = NODE_ADD;
            break;
        case '-':
            type = NODE_SUB;
            break;
        case '*':
            type = NODE_MUL;
            break;
        case '/':
            type = NODE_DIV;
            break;
        case '^':
            type = NODE_EXP;
            break;
    }

    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = type;
    node->val.op = token.val.op;

    return node;
}

node_t* create_function(token_t token) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_FUNCTION;
    node->val.id = string_clone(&token.val.name);

    return node;
}

node_t* create_tree(parser_t* parser) {
    stack_s node_stack = create_stack(sizeof(node_t*));
    
    while (!is_empty(&parser->output)) {
        token_t token = *(token_t*)dequeue(&parser->output);
        
        if (token.type == NUM) {
            node_t* node = create_constant(token); 
            stack_push(&node_stack, &node);
        } else if (token.type == FUNC) {
            node_t* node = create_function(token);
            node_t* child = *(node_t**)stack_pop(&node_stack);
            node->left = child;
            stack_push(&node_stack, &node);
        } else if (token.type == VAR) {
            node_t* node = create_variable(token);
            stack_push(&node_stack, &node);
        } else if (token.type == OP) {
            node_t* right = *(node_t**)stack_pop(&node_stack);
            node_t* left = *(node_t**)stack_pop(&node_stack);
            node_t* node = create_op(token);
            node->right = right;
            node->left = left;
            stack_push(&node_stack, &node);
        } else {
            fprintf(stderr, "unknown token type");
        }
    }

    return *(node_t**)stack_top(&node_stack);
}

void print_node(node_t* node) {
    if (node->type == NODE_CONSTANT) {
        printf("%f, ", node->val.num);
    } else if (node->type == NODE_FUNCTION || node->type == NODE_VARIABLE) {
        string_print(node->val.id, 1);
        printf(", ");
    } else if (node->type == NODE_ADD || node->type == NODE_SUB || node->type == NODE_MUL || node->type == NODE_DIV || node->type == NODE_EXP) {
        printf("%c, ", node->val.op);
    } else {
        printf("unknown type, ");
    }
}

void print_tree(node_t* node) {
    if (!node) return;

    print_tree(node->left);
    print_tree(node->right);
    print_node(node);
}


