#include <stdio.h>
#include <unistd.h>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "lexer/Token.h"
#include "interpreter/Context.h"
#include "interpreter/Expr.h"

int main() {
    Lexer lexer("test.s");
    std::vector<Token> tokens = lexer.make_tokens();
    //lexer.print_tokens();

    SymbolTable* globalVarTable = new SymbolTable("var");
    SymbolTable* funcTable = new SymbolTable("func");

    Parser parser("test.s", tokens, globalVarTable, funcTable);
    parser.match_program();

    Context cxt(globalVarTable, funcTable);
    CallNodeEx* main = (CallNodeEx*)(funcTable->get("main"));
    int result = main->eval(cxt);

    printf("result is %d\n", result);

    return 1;
}
