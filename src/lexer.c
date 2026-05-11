#include "lexer.h"

vector_t functions;

double sin_func(double x) {
    return sin(x);
}

double cos_func(double x) {
    return cos(x);
}

double tan_func(double x) {
    return tan(x);
}

double ln_func(double x) {
    return log(x);
}

double sqrt_func(double x) {
    return sqrt(x);
}

static void functions_init() {
    functions = vector_create(sizeof(function_t)); 
    function_t sin_f = { .name = "SIN", .func = sin_func };
    function_t cos_f = { .name = "COS", .func = cos_func };
    function_t tan_f = { .name = "TAN", .func = tan_func };
    function_t ln_f = { .name = "LN", .func = ln_func };
    function_t sqrt_f = { .name = "SQRT", .func = sqrt_func };
    vector_push_back(&functions, &sin_f);
    vector_push_back(&functions, &cos_f);
    vector_push_back(&functions, &tan_f);
    vector_push_back(&functions, &ln_f);
    vector_push_back(&functions, &sqrt_f);
}

lexer_t create_lexer(const char* expr) {
    lexer_t lexer = {
        .pos = 1,
        .curr_char = expr[0],
        .expr = expr,
        .len = strlen(expr)
    };
   
    functions_init();

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

    for (int i = 0; i < functions.size; i++) {
        function_t* func = (function_t*)vector_get(&functions, i);
        char* function = func->name;
        if (string_compare_literal(&upper_given, function) == 0) {
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
