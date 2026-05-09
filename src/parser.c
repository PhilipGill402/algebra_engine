#include "parser.h"

/* HELPERS */
int operator_precedence(token_t token) {
    if (token.type == FUNC) {
        return 5;
    } else if (token.type == OP) {
        if (token.val.op == '^') {
            return 4;
        } else if (token.val.op == '*' || token.val.op == '/') {
            return 2;
        } else if (token.val.op == '+' || token.val.op == '-') {
            return 1;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

int is_right_associative(token_t token) {
    if (token.type != OP) {
        return 0;
    }

    if (token.val.op == '^') {
        return 1;
    } else {
        return 0;
    }
}

double node_sin(node_t* node) {
    return sin(node->value);
}

double node_cos(node_t* node) {
    return cos(node->value);
}

double node_tan(node_t* node) {
    return tan(node->value);
}

double node_sqrt(node_t* node) {
    return sqrt(node->value);
}

void backward_sin(node_t* self) {
    self->left->grad += cos(self->left->value) * self->grad;
}

void backward_cos(node_t* self) {
    self->left->grad += -sin(self->left->value) * self->grad;
}

void backward_tan(node_t* self) {
    double sec = 1.0 / cos(self->left->value);
    self->left->grad += sec * sec * self->grad;
}

void backward_sqrt(node_t* self) {
    double x = 0.5 / sqrt(self->left->value);
    self->left->grad += x * self->grad;
}

/* PARSER */
parser_t init_parser(arena_t* arena) {
    parser_t parser;
    parser.tokens = create_queue(sizeof(token_t));
    parser.op_stack = create_stack(sizeof(token_t));
    parser.output = create_queue(sizeof(token_t));
    parser.variables = vector_create(sizeof(entry_t));
    parser.nodes = vector_create(sizeof(node_t*));
    parser.lexer = NULL;

    return parser;
}

void get_infix(parser_t* parser) {
    token_t token = get_next_token(parser->lexer);
    enqueue(&parser->tokens, &token);

    while (token.type != ENDOFFILE) {
        token = get_next_token(parser->lexer);
        if (token.type == ENDOFFILE) {
            break;
        }

        enqueue(&parser->tokens, &token);
    }

}

void get_postfix(parser_t* parser) {
    while (!is_empty(&parser->tokens)) {
        token_t token = *(token_t*)dequeue(&parser->tokens);
        
        if (token.type == NUM) {
            enqueue(&parser->output, &token);
        } else if (token.type == VAR) {
            enqueue(&parser->output, &token);
        } else if (token.type == FUNC) {
            stack_push(&parser->op_stack, &token);
        } else if (token.type == OP) {
            while (!stack_is_empty(&parser->op_stack)
                    && ((*(token_t*)stack_top(&parser->op_stack)).type == OP) 
                    && ((operator_precedence(*(token_t*)stack_top(&parser->op_stack)) > operator_precedence(token))
                        || ((operator_precedence(*(token_t*)stack_top(&parser->op_stack)) == operator_precedence(token)) && !is_right_associative(token)))) {
                token_t op = *(token_t*)stack_pop(&parser->op_stack);
                enqueue(&parser->output, &op);
            }

            stack_push(&parser->op_stack, &token);
        } else if (token.type == LPAREN) {
            stack_push(&parser->op_stack, &token);
        } else if (token.type == RPAREN) {
            while ((*(token_t*)stack_top(&parser->op_stack)).type != LPAREN) {
                token_t op = *(token_t*)stack_pop(&parser->op_stack);
                enqueue(&parser->output, &op);
            }

            stack_pop(&parser->op_stack);

            if (!stack_is_empty(&parser->op_stack) && (*(token_t*)stack_top(&parser->op_stack)).type == FUNC) {
                token_t fn = *(token_t*)stack_pop(&parser->op_stack);
                enqueue(&parser->output, &fn);
            }
        }
    }

    while (!stack_is_empty(&parser->op_stack)) {
        token_t op = *(token_t*)stack_pop(&parser->op_stack);
        enqueue(&parser->output, &op);
    }
}




void set_expr(const char* expr, parser_t* parser) {
    lexer_t* lexer = reserve(sizeof(lexer_t)); 
    *lexer = create_lexer(expr);

    parser->lexer = lexer;
}

node_t* parse(parser_t* parser) {
    get_infix(parser);
    get_postfix(parser);

    node_t* root = create_graph(parser);
    backprop(root, &parser->nodes);

    return root;
}

void create_var(parser_t* parser, char* name) {
    string_t str_name = string_literal(name); 
    create_var_node(&str_name, &parser->variables); 
}

void set_var(parser_t* parser, char* name, double val) {
    string_t str_name = string_literal(name);
    
    for (int i = 0; i < parser->variables.size; i++) {
        entry_t entry = *(entry_t*)get(&parser->variables, i);
        if (string_compare(&str_name, &entry.name) == 0) {
            entry.node->value = val;
            return;
        }
    }
    
    node_t* node = reserve(sizeof(node_t));
    node->value = val;
    node->grad = 0;
    node->left = NULL;
    node->right = NULL;
    node->backward = NULL;
    node->is_var = 1;
    node->name = str_name;

    entry_t entry = {
        .name = str_name,
        .node = node 
    };

    vector_push_back(&parser->variables, &entry);
}

