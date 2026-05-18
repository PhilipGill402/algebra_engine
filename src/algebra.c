#include "algebra.h"

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
                // NOT AN IDENTITY 
                node->can_be_simplified = 0;
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
                // NOT AN IDENTITY 
                node->can_be_simplified = 0;
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

static void simplify_function(node_t* node) {
    if (!node->can_be_simplified && node->left->type != NODE_CONSTANT)
        return;
    
    double val = 0;
    string_t upper = string_upper(node->val.id);
    node_t* left = node->left;

    if (left->type != NODE_CONSTANT) return;
    
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
    node->can_be_simplified = 1;
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

    can_be_simplified(root);
    
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

/* DIFFERENTIATING FUNCTIONS */

node_t* diff_constant(node_t* node) {
    return create_constant(0); 
}

node_t* diff_var(node_t* node) {
    return create_constant(1);
}

node_t* diff_op(node_t* node) {
    node_t* op = create_op(node->val.op);

    if (node->val.op == '+') {
        // f(x) = g(y) + h(z) => f'(x) = g'(y) + h'(z) 

        op->left = diff_tree(node->left);
        op->right = diff_tree(node->right);
    } else if (node->val.op == '-') {
        // f(x) = g(y) + h(z) => f'(x) = g'(y) - h'(z)

        op->left = diff_tree(node->left);
        op->right = diff_tree(node->right);
    } else  if (node->val.op == '*') {
        // f(x) = g(y) * h(z) => f'(x) = g(y) * h'(z) + g'(y) * h(z) 

        op->val.op = '+';

        node_t* left = node_clone(node->left);
        node_t* right = node_clone(node->right);
        node_t* left_diff = diff_tree(left);
        node_t* right_diff = diff_tree(right);

        node_t* left_mult = create_op('*');
        left_mult->left = node_clone(left);
        left_mult->right = node_clone(right_diff);
        
        node_t* right_mult = create_op('*');
        right_mult->left = node_clone(left_diff);
        right_mult->right = node_clone(right);

        op->left = left_mult;
        op->right = right_mult;

    } else if (node->val.op == '/') {
        // f(x) = g(y) / h(z) => f'(x) = (g'(y) * h(z) - g(y) * h'(z)) / h(z) ^ 2
        
        node_t* left = node_clone(node->left);
        node_t* right = node_clone(node->right);
        node_t* left_diff = diff_tree(left);
        node_t* right_diff = diff_tree(right);
        
        node_t* numerator = create_op('-');

        node_t* left_numerator = create_op('*');
        left_numerator->left = node_clone(left_diff);
        left_numerator->right = node_clone(right);
        
        node_t* right_numerator = create_op('*');
        right_numerator->left = node_clone(left);
        right_numerator->right = node_clone(right_diff);

        numerator->left = left_numerator;
        numerator->right = right_numerator;
        
        node_t* denominator = create_op('^');
        denominator->left = node_clone(right);
        denominator->right = create_constant(2);
        
        op->left = numerator;
        op->right = denominator;
    } else if (node->val.op == '^') {
        // f(x) = g(y) ^ h(z) => g(y) ^ h(z) * (h'(z) * ln(g(y)) + (h(z) * g'(y)) / g(y))
        op->val.op = '*'; 
        node_t* left = node_clone(node->left);
        node_t* right = node_clone(node->right);
        node_t* left_diff = diff_tree(left);
        node_t* right_diff = diff_tree(right);

        node_t* left_op = create_op('^');
        left_op->left = node_clone(left);
        left_op->right = node_clone(right);

        node_t* right_op = create_op('+');
        
        node_t* right_op_left = create_op('*');
        
        string_t* ln_str = string_literal("ln");
        node_t* ln_node = create_function(ln_str);
        ln_node->left = node_clone(left);
    
        right_op_left->left = node_clone(right_diff);
        right_op_left->right = ln_node;

        node_t* right_op_right = create_op('/');

        node_t* right_op_right_numerator = create_op('*');
        right_op_right_numerator->left = node_clone(right);
        right_op_right_numerator->right = node_clone(left_diff);

        node_t* right_op_right_denominator = node_clone(left);

        right_op_right->left = right_op_right_numerator;
        right_op_right->right = right_op_right_denominator;

        right_op->left = right_op_left;
        right_op->right = right_op_right;
        
        op->left = left_op;
        op->right = right_op;
    } else {
        op = NULL; 
        fprintf(stderr, "unknown operator\n");
    } 

    return op;
}

node_t* diff_function(node_t* node) {
    node_t* func_diff = NULL;
    node_t* root = create_op('*');
    string_t upper = string_upper(node->val.id);
    node_t* left = node->left;
    
    root->right = diff_tree(left);
    
    for (int i = 0; i < functions.size; i++) {
        function_t* func = (function_t*)vector_get(&functions, i);
        if (string_compare_literal(&upper, func->name) == 0) {
            func_diff = func->diff(node);
            break;
        }
    }

    root->left = func_diff;

    return root;
}

node_t* diff_tree(node_t* root) {
    if (!root) return NULL;
    node_t* new_root = NULL;

    if (root->type == NODE_CONSTANT) {
        new_root = diff_constant(root);  
    } else if (root->type == NODE_VARIABLE) {
        new_root = diff_var(root); 
    } else if (root->type == NODE_FUNCTION) {
        new_root = diff_function(root);
    } else if (root->type == NODE_OP) {
        new_root = diff_op(root);
    }

    return new_root;
}


