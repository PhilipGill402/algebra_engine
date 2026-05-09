#include "tree.h"

node_t* create_graph(parser_t* parser) {
    stack_s node_stack = create_stack(sizeof(node_t*));
    
    while (!is_empty(&parser->output)) {
        token_t token = *(token_t*)dequeue(&parser->output);

        if (token.type == NUM) {
            node_t* node = create_node(token.val.num);
            stack_push(&node_stack, &node);
            vector_push_back(&parser->nodes, &node);
        } else if (token.type == VAR) {
            node_t* node = create_var_node(&token.val.name, &parser->variables);
            stack_push(&node_stack, &node);
            vector_push_back(&parser->nodes, &node);
        } else if (token.type == OP) {
            node_t* right = *(node_t**)stack_pop(&node_stack);
            node_t* left = *(node_t**)stack_pop(&node_stack);
            node_t* node;

            switch (token.val.op) {
                case '+':
                    node = node_add(left, right);
                    stack_push(&node_stack, &node);
                    vector_push_back(&parser->nodes, &node);
                    break;
                case '*':
                    node = node_mul(left, right);
                    stack_push(&node_stack, &node);
                    vector_push_back(&parser->nodes, &node);
                    break;
                case '-':
                    node = node_sub(left, right);
                    stack_push(&node_stack, &node);
                    vector_push_back(&parser->nodes, &node);
                    break;
                case '/':
                    node = node_div(left, right);
                    stack_push(&node_stack, &node);
                    vector_push_back(&parser->nodes, &node);
                    break;
                case '^':
                    node = node_pow(left, right);
                    stack_push(&node_stack, &node);
                    vector_push_back(&parser->nodes, &node);
                    break;
                default:
                    fprintf(stderr, "Did not understand operator '%c' in 'create_graph'\n", token.val.op); 
                    break;
            } 
        } else if (token.type == FUNC) {
            string_t sin_str = string_literal("SIN");
            string_t cos_str = string_literal("COS");
            string_t tan_str = string_literal("TAN");
            string_t sqrt_str = string_literal("SQRT");
            string_t upper_str = string_upper(&token.val.name);
            node_t* left = *(node_t**)stack_pop(&node_stack);
            node_t* node;

            if (string_compare_literal(&upper_str, "SIN") == 0) {
                node = node_func(left, node_sin, backward_sin);
                stack_push(&node_stack, &node);
                vector_push_back(&parser->nodes, &node);
            } else if (string_compare_literal(&upper_str, "COS") == 0) {
                node = node_func(left, node_cos, backward_cos);
                stack_push(&node_stack, &node);
                vector_push_back(&parser->nodes, &node);
            } else if (string_compare_literal(&upper_str, "TAN") == 0) {
                node = node_func(left, node_tan, backward_tan);
                stack_push(&node_stack, &node);
                vector_push_back(&parser->nodes, &node);
            } else if (string_compare_literal(&upper_str, "SQRT") == 0) {
                node = node_func(left, node_sqrt, backward_sqrt);
                stack_push(&node_stack, &node);
                vector_push_back(&parser->nodes, &node);
            } else {
                string_print(&token.val.name, 0);
                fprintf(stderr, "Function is not defined\n");
            }
        }
    }
    
    return *(node_t**)stack_top(&node_stack);
}
