#ifndef INCLUDE_LEXER_H_
#define INCLUDE_LEXER_H_

#include <ctype.h>
#include <math.h>
#include "functions.h"
#include "string_t.h"
#include "token.h"
#include "vector.h"


typedef struct {
    int pos;
    char curr_char;
    const char* expr;
    int len;
    vector_t functions;
} lexer_t;

lexer_t create_lexer(const char* expr);
token_t get_next_token(lexer_t* lexer);

#endif // !INCLUDE_LEXER_H_
