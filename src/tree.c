#include "tree.h"

static node_t* create_constant(token_t token) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_CONSTANT;
    node->val.num = token.val.num;
    node->can_be_simplified = 1;

    return node;
}

static node_t* create_variable(token_t token) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_VARIABLE;
    node->val.id = string_clone(&token.val.name);
    node->can_be_simplified = 1;

    return node;
}

static node_t* create_op(token_t token) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_OP;
    node->val.op = token.val.op;
    node->can_be_simplified = 1;

    return node;
}

static node_t* create_function(token_t token) {
    node_t* node = malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_FUNCTION;
    node->val.id = string_clone(&token.val.name);
    node->can_be_simplified = 1;

    return node;
}

void node_free(node_t* node) {
    if (!node) return;

    node_free(node->left);
    node_free(node->right);
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

    node_t* root = *(node_t**)stack_top(&node_stack); 
    can_be_simplified(root);

    return root;
}

void print_inorder_tree(node_t* node) {
    if (!node) return;

    if (node->type == NODE_CONSTANT) {
        printf("%f", node->val.num);
    } else if (node->type == NODE_VARIABLE) {
        string_print(node->val.id, 1);
    } else if (node->type == NODE_FUNCTION) {
        string_print(node->val.id, 1);
        printf("(");
        print_inorder_tree(node->left);   // unary argument
        printf(")");
    } else if (node->type == NODE_OP) {
        printf("("); 
        print_inorder_tree(node->left);
        printf("%c", node->val.op);
        print_inorder_tree(node->right);
    printf(")");
    } else {
        printf("unknown");
    }
}

