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

uint8_t can_be_simplified(node_t* node) {
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
        uint8_t left_simpl = can_be_simplified(node->left);
        node->can_be_simplified = left_simpl == 1 ? 1 : 0;
        return node->can_be_simplified;
    } else if (node->type == NODE_OP) {
        // basic simplification cases (identities and zero multiplication)        
        node_t* left = node->left;
        node_t* right = node->right;
        
        // additive identity
        if (node->val.op == '+') {
            if (left->type == NODE_CONSTANT && left->val.num == 0) {
                node->can_be_simplified = 2;
                return node->can_be_simplified;
            } else if (right->type == NODE_CONSTANT && right->val.num == 0) {
                node->can_be_simplified = 2;
                return node->can_be_simplified;
            }

        }
        // subtraction identity
        else if (node->val.op == '-') {
            if (left->type == NODE_CONSTANT && left->val.num == 0) {
                node->can_be_simplified = 2;
                return node->can_be_simplified;
            } else if (right->type == NODE_CONSTANT && right->val.num == 0) {
                node->can_be_simplified = 2;
                return node->can_be_simplified;
            }
        }
        // multiplicative identity and multiplication by 0
        else if (node->val.op == '*') {
            if (left->type == NODE_CONSTANT && (left->val.num == 1 || left->val.num == 0)) {
                node->can_be_simplified = 2;
                return node->can_be_simplified;
            } else if (right->type == NODE_CONSTANT && (right->val.num == 1 || right->val.num == 0)) {
                node->can_be_simplified = 2;
                return node->can_be_simplified;
            }
        }
        // division identity
        else if (node->val.op == '/') {
            if (left->type == NODE_CONSTANT && left->val.num == 1) {
                node->can_be_simplified = 2;
                return node->can_be_simplified;
            } else if (right->type == NODE_CONSTANT && right->val.num == 1) {
                node->can_be_simplified = 2;
                return node->can_be_simplified;
            }
        }
        // else decide based on simplification of child nodes
        uint8_t left_simpl = can_be_simplified(left);
        uint8_t right_simpl = can_be_simplified(right);
        node->can_be_simplified = left_simpl <= right_simpl ? left_simpl : right_simpl;
        
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
    if (!node->can_be_simplified && node->left->type != NODE_CONSTANT)
        return;
    
    double val = 0;
    string_t upper = string_upper(node->val.id);
    node_t* left = node->left;
    
    for (int i = 0; i < functions.size; i++) {
        function_t* func = (function_t*)vector_get(&functions, i);
        if (string_compare_literal(&upper, func->name) == 0) {
            val = func->func(left->val.num);
            break;
        }
    }

    node_free(node->left);
    node_free(node->right);

    node->left = NULL;
    node->right = NULL;
    node->type = NODE_CONSTANT;
    node->val.num = val;
    node->can_be_simplified = 1;
}

static void replace_node_with(node_t* node, node_t* replacement, node_t* discard) {
    node_t replacement_copy = *replacement;

    replacement->left = NULL;
    replacement->right = NULL;

    node_free(discard);
    node_free(replacement);

    *node = replacement_copy;
}

static void replace_node_with_constant(node_t* node, double value) {
    node_free(node->left);
    node_free(node->right);

    node->left = NULL;
    node->right = NULL;
    node->type = NODE_CONSTANT;
    node->val.num = value;
    node->can_be_simplified = 0;
}

static void simplify_identity(node_t* node) { 
    node_t* var;
    node_t* constant;

    if (node->right->type == NODE_CONSTANT && (node->val.op == '+' && node->right->val.num == 0)) {
        constant = node->right;
        var = node->left;
    } else if (node->right->type == NODE_CONSTANT && (node->val.op == '-' && node->right->val.num == 0)) {
        constant = node->right;
        var = node->left;
    } else if (node->right->type == NODE_CONSTANT && (node->val.op == '*' && node->right->val.num == 0)) {
        constant = node->right;
        var = node->left;
    } else if (node->right->type == NODE_CONSTANT && (node->val.op == '*' && node->right->val.num == 1)) {
        constant = node->right;
        var = node->left;
    } else if (node->right->type == NODE_CONSTANT && (node->val.op == '/' && node->right->val.num == 1)) {
        constant = node->right;
        var = node->left;
    } else if (node->left->type == NODE_CONSTANT && (node->val.op == '+' && node->left->val.num == 0)) {
        var = node->right;
        constant = node->left;
    } 
    /*
    else if (node->left->type == NODE_CONSTANT && (node->val.op == '-' && node->left->val.num == 0)) {
        var = node->right;
        constant = node->left;
    */
    
    else if (node->left->type == NODE_CONSTANT && (node->val.op == '*' && node->left->val.num == 0)) {
        var = node->right;
        constant = node->left;
    } else if (node->left->type == NODE_CONSTANT && (node->val.op == '*' && node->left->val.num == 1)) {
        var = node->right;
        constant = node->left;
    }
    
    /*
    else if (node->left->type == NODE_CONSTANT && (node->val.op == '/' && node->left->val.num == 1)) {
        var = node->right;
        constant = node->left;
    }
    */

    // handles all identities
    if (node->val.op != '*' || constant->val.num != 0) {
        // swap the op and variable node then destroy
        replace_node_with(node, var, constant);
    } 
    // handle multiplication by 0
    else {
        replace_node_with_constant(node, 0); 
    }
}

static void simplify_op(node_t* node) {
    if (!node->can_be_simplified && (node->left->type != NODE_CONSTANT || node->right->type != NODE_CONSTANT))
        return;

    double val = 0;
    node_t* left = node->left;
    node_t* right = node->right;
    
    // simplify identities
    if (left->type != NODE_CONSTANT || right->type != NODE_CONSTANT) {
        simplify_identity(node);
        print_inorder_tree(node);
        printf("\n");
        return;
    }

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

    if (root->type == NODE_CONSTANT) {
        return;
    } else if (root->type == NODE_VARIABLE) {
        return;
    } else if (root->type == NODE_FUNCTION) {
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
        printf("%c", node->val.op);
        print_inorder_tree(node->right);
    } else {
        printf("unknown");
    }
}

