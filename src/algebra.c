#include "algebra.h"
/*
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

static void replace_node_with(node_t* node, node_t* replacement, node_t* discard) {
    node_t replacement_copy = *replacement;

    replacement->left = NULL;
    replacement->right = NULL;
    if (replacement->type == NODE_VARIABLE || replacement->type == NODE_FUNCTION) {
        string_t* copy = string_clone(replacement->val.id);
        replacement_copy.val.id = copy;
    }


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
    if (!node->can_be_simplified)
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
    printf("%d\n", root->type);
    
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

*/
