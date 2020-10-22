#include <stdio.h>
#include <unistd.h>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "lexer/Token.h"

int main() {
    Lexer lexer("test.s");
    std::vector<Token> tokens = lexer.make_tokens();
    //lexer.print_tokens();

    Parser parser(tokens);

    parser.make_ast();

    while (1) sleep(2);

    return 1;
}
