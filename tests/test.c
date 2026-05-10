#include <stdio.h>

#include "tree.h"
#include "algebra.h"
#include "lexer.h"
#include "token.h"
#include "parser.h"

int main() {
    char* equation = "3*ln(1*9)-5/x^2";
    //char* equation = "3 + 3 * 9\n";
    parser_t parser = init_parser();
    set_expr(equation, &parser);
    parse(&parser);

    node_t* root = create_tree(&parser);
    simplify_tree(root);
    print_inorder_tree(root);
    printf("\n");

    return 0;
}
