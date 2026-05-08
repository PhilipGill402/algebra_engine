#include "lexer.h"

lexer_t create_lexer(const char* expr) {
    lexer_t lexer = {
        .pos = 1,
        .curr_char = expr[0],
        .expr = expr,
        .len = strlen(expr),
        .functions = vector_create(sizeof(string_t))
    };
   
    string_t name = string_literal("SIN");
    vector_push_back(&lexer.functions, &name);
    string_free(&name);
    
    name = string_literal("COS");
    vector_push_back(&lexer.functions, &name);
    string_free(&name);

    name = string_literal("TAN");
    vector_push_back(&lexer.functions, &name);
    string_free(&name);

    name = string_literal("SQRT");
    vector_push_back(&lexer.functions, &name);
    string_free(&name);

    name = string_literal("LN");
    vector_push_back(&lexer.functions, &name);
    string_free(&name);

    return lexer;
}

void advance(lexer_t* lexer) {
    if (lexer->pos >= lexer->len) {
        lexer->curr_char = '\0';
        return;
    }

    lexer->curr_char = lexer->expr[lexer->pos++];
}

int is_function(lexer_t* lexer, string_t* func) {
    string_t upper_given = string_upper(func); 

    for (int i = 0; i < lexer->functions.size; i++) {
        string_t function = *(string_t*)vector_get(&lexer->functions, i); 
        if (string_compare(&upper_given, &function) == 0) {
            return 1;
        }
    }

    return 0;
}

token_t get_next_token(lexer_t* lexer) {
    while (lexer->curr_char != '\0') {
        if (isspace((unsigned char)lexer->curr_char)) {
            advance(lexer); 
        } else if (isdigit(lexer->curr_char)) {
            string_t val = string_create();
            
            while (isdigit(lexer->curr_char)) {
                string_append_chr(&val, lexer->curr_char);
                advance(lexer);
            }

            if (lexer->curr_char == '.') {
                string_append_chr(&val, '.');
                advance(lexer);
                while (isdigit(lexer->curr_char)) {
                    string_append_chr(&val, lexer->curr_char);
                    advance(lexer);
                }
            }

            double num = string_to_double(&val);
            return create_num_token(num);
        } else if ((unsigned char)isalpha(lexer->curr_char)) {
            string_t val = string_create();

            while (isalpha(lexer->curr_char)) {
                string_append_chr(&val, lexer->curr_char);
                advance(lexer);
            }

            if (is_function(lexer, &val)) {
                return create_func_token(val);
            }

            return create_var_token(val);
        } else if (lexer->curr_char == '+') {
            advance(lexer);
            return create_op_token('+');
        } else if (lexer->curr_char == '-') {
            advance(lexer);
            return create_op_token('-');
        } else if (lexer->curr_char == '*') {
            advance(lexer);
            return create_op_token('*');
        } else if (lexer->curr_char == '/') {
            advance(lexer);
            return create_op_token('/');
        } else if (lexer->curr_char == '^') {
            advance(lexer);
            return create_op_token('^');
        } else if (lexer->curr_char == '(') {
            advance(lexer);
            return create_token(LPAREN, '(');
        } else if (lexer->curr_char == ')') {
            advance(lexer);
            return create_token(RPAREN, ')');
        } else {
            advance(lexer); 
            token_t token = {
                .type = UNKNOWN,
                .val.name = string_literal("UNKNOWN")
            };

            return token; 
        }
    }

    token_t token = {
        .type = ENDOFFILE,
        .val.name = string_literal("eof")
    };

    return token;
}
