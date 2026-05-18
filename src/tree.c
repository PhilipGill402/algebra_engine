#include "tree.h"

node_t* create_tree(parser_t* parser) {
    stack_s node_stack = create_stack(sizeof(node_t*));
    
    while (!is_empty(&parser->output)) {
        token_t token = *(token_t*)dequeue(&parser->output);
        
        if (token.type == NUM) {
            node_t* node = create_constant(token.val.num); 
            stack_push(&node_stack, &node);
        } else if (token.type == FUNC) {
            node_t* node = create_function(string_clone(&token.val.name));
            node_t* child = *(node_t**)stack_pop(&node_stack);
            node->left = child;
            stack_push(&node_stack, &node);
        } else if (token.type == VAR) {
            node_t* node = create_variable(string_clone(&token.val.name));
            stack_push(&node_stack, &node);
        } else if (token.type == OP) {
            node_t* right = *(node_t**)stack_pop(&node_stack);
            node_t* left = *(node_t**)stack_pop(&node_stack);
            node_t* node = create_op(token.val.op);
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

