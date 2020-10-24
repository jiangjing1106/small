#include <stdio.h>
#include <unistd.h>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "lexer/Token.h"

int main() {
    Lexer lexer("test.s");
    std::vector<Token> tokens = lexer.make_tokens();
    //lexer.print_tokens();

    SymbolTable* globalVarTable = new SymbolTable("var");
    SymbolTable* funcTable = new SymbolTable("func");

    Parser parser("test.s", tokens, globalVarTable, funcTable);
    parser.match_program();

    

    while (1) sleep(2);

    return 1;
}
