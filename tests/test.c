#include <stdio.h>

#include "tree.h"
#include "algebra.h"
#include "lexer.h"
#include "token.h"
#include "parser.h"

int main() {
    char* equation = "3*ln(x+1)-5/x^2";
    
    parser_t parser = init_parser();
    set_expr(equation, &parser);
    parse(&parser);

    node_t* root = create_tree(&parser);

    print_tree(root);

    return 0;
}
