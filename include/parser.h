#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include <math.h>
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include "string_t.h"
#include "queue.h"
#include "stack.h"
#include "vector.h"

typedef struct {
    queue_t tokens;
    stack_s op_stack;
    queue_t output;
    vector_t variables;
    vector_t nodes;
    lexer_t* lexer;
} parser_t;

parser_t init_parser();
void set_expr(const char* expr, parser_t* parser);
void get_infix(parser_t* parser);
void get_postfix(parser_t* parser);
void parse(parser_t* parser);
void create_var(parser_t* parser, char* name);
void set_var(parser_t* parser, char* name, double val);

#endif // !INCLUDE_PARSER_H_
