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

static void node_free(node_t* node) {
    if (!node) return;

    node_free(node->left);
    node_free(node->right);
    free(node);
}

static uint8_t can_be_simplified(node_t* node) {
    if (!node) {
        fprintf(stderr, "unexpected null node\n");
        return 0;
    }

    if (node->type == NODE_CONSTANT) {
        node->can_be_simplified = 1;
        return 1;
    } else if (node->type == NODE_VARIABLE) {
        node->can_be_simplified = 0;
        return 0;
    } else if (node->type == NODE_FUNCTION) {
        //node->can_be_simplified = 0; 
        node->can_be_simplified = can_be_simplified(node->left);
        return node->can_be_simplified;
    } else if (node->type == NODE_OP) {
        // basic simplification cases (identities and zero multiplication)        
        node_t* left = node->left;
        node_t* right = node->right;
        

        uint8_t left_simpl = can_be_simplified(left);
        uint8_t right_simpl = can_be_simplified(right);
        node->can_be_simplified = left <= right ? left : right;
        return node->can_be_simplified;
    }
    
    fprintf(stderr, "unknown node type: %d\n", node->type);
    node->can_be_simplified = 0;
    return 0;
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

static void simplify_function(node_t* node) {
    if (!node->can_be_simplified)
        return;
    
    double val = 0;
    string_t upper = string_upper(node->val.id);
    node_t* left = node->left;
    if (string_compare_literal(&upper, "LN") == 0) {
        val = log(left->val.num);
    }

    node_free(node->left);
    node_free(node->right);

    node->left = NULL;
    node->right = NULL;
    node->type = NODE_CONSTANT;
    node->val.num = val;
    node->can_be_simplified = 1;
}

static void simplify_op(node_t* node) {
    if (!node->can_be_simplified)
        return;

    double val = 0;
    node_t* left = node->left;
    node_t* right = node->right;

    if (node->val.op == '+') {
        val = left->val.num + right->val.num;
    } else if (node->val.op == '-') {
        val = left->val.num - right->val.num;
    } else  if (node->val.op == '*') {
        val = left->val.num * right->val.num;
    } else if (node->val.op == '/') {
        val = left->val.num / right->val.num;
    } else if (node->val.op == '^') {
        val = pow(left->val.num, right->val.num);
    } else {
        fprintf(stderr, "unknown operator\n");
    }

    node_free(node->left);
    node_free(node->right);

    node->left = NULL;
    node->right = NULL;
    node->type = NODE_CONSTANT;
    node->val.num = val;
    node->can_be_simplified = 1;
}

void simplify_tree(node_t* root) {
    if (!root) return;

    simplify_tree(root->left);
    simplify_tree(root->right);

    if (root->type == NODE_CONSTANT)
        return;
    else if (root->type == NODE_VARIABLE)
        return;
    else if (root->type == NODE_FUNCTION) {
        simplify_function(root);
        return;
    } else if (root->type == NODE_OP) {
        simplify_op(root);
        return;
    }
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
        print_inorder_tree(node->left);
        printf(" %c ", node->val.op);
        print_inorder_tree(node->right);
    } else {
        printf("unknown");
    }
}





