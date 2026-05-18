#include <stdio.h>

#include "tree.h"
#include "algebra.h"
#include "lexer.h"
#include "token.h"
#include "parser.h"

int main() {
    char* equation = "0-x";
    //char* equation = "3 + 3 * 9\n";
    parser_t parser = init_parser();
    set_expr(equation, &parser);
    parse(&parser);

    node_t* root = create_tree(&parser);
    simplify_tree(root);
    printf("f(x) = "); 
    print_inorder_tree(root);
    printf("\n");
    node_t* diff = diff_tree(root);
    can_be_simplified(diff);
    simplify_tree(diff);
    printf("f'(x) = ");
    print_inorder_tree(diff);
    printf("\n");
    
    return 0;
}
