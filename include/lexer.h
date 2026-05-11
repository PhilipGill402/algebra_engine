#ifndef INCLUDE_LEXER_H_
#define INCLUDE_LEXER_H_

#include <ctype.h>
#include <math.h>
#include "string_t.h"
#include "token.h"
#include "vector.h"

typedef struct {
    char* name;
    double (*func)(double x);
} function_t;

typedef struct {
    int pos;
    char curr_char;
    const char* expr;
    int len;
    vector_t functions;
} lexer_t;

lexer_t create_lexer(const char* expr);
token_t get_next_token(lexer_t* lexer);

extern vector_t functions;

#endif // !INCLUDE_LEXER_H_
