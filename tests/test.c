#include <stdio.h>

#include "tree.h"
#include "algebra.h"
#include "lexer.h"
#include "token.h"

int main() {
    char* equation = "3ln(x+1)-5/x^2";
    
    lexer_t lexer = create_lexer(equation);
    token_t token = get_next_token(&lexer);

    while (token.type != ENDOFFILE) {
        print_token(&token);
        printf("\n");
        token = get_next_token(&lexer);
    }

    return 0;
}
