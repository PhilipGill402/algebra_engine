#include "token.h"

token_t create_num_token(double num) {
    token_t token = {
        .type = NUM,
        .val.num = num
    };

    return token;
}

token_t create_op_token(char op) {
    token_t token = {
        .type = OP,
        .val.op = op
    };

    return token;
}

token_t create_func_token(string_t name) {
    token_t token = {
        .type = FUNC,
        .val.name = name
    };

    return token;
}

token_t create_var_token(string_t name) {
    token_t token = {
        .type = VAR,
        .val.name = name 
    };

    return token;
}

token_t create_token(token_type_t type, char c) {
    token_t token = {
        .type = type,
        .val.op = c
    };

    return token;
}

void print_token(void* token) {
    token_t t = *(token_t*)token; 
    printf("TOKEN(");
    
    switch (t.type) {
        case NUM:
            printf("NUM, %f", t.val.num);
            break;
        case OP:
            printf("OP, %c", t.val.op);
            break;
        case FUNC:
            printf("FUNC, ");
            string_print(&t.val.name, 1);
            break;
        case VAR:
            printf("VAR, ");
            string_print(&t.val.name, 1);
            break;
        case LPAREN:
            printf("LPAREN, %c", t.val.op);
            break;
        case RPAREN:
            printf("RPAREN, %c", t.val.op);
            break;
        case ENDOFFILE:
            printf("ENDOFFILE, eof");
            break;
        default:
            printf("UNKNOWN, UNKNOWN");
            break;
    }
    
    printf(")");
}
