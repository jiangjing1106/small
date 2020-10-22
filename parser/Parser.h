#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <stack>

#include "../lexer/Token.h"
#include "AST.h"

typedef struct ParseResult_t {
    ASTree* node;
    int err_code;
    std::string err_msg;
} ParseResult;

class Parser {
public:
    Parser(std::vector<Token> tokens);
    ~Parser();

    ASTree* make_ast();

private:

    typedef ASTree* (Parser::*func)();

    void advance();
    void fallback();
   // ASTree* match_program();
   // ASTree* match_statement();
    ASTree* match_operatorExpr();
    ASTree* match_factor();
    ASTree* match_unitaryExpr();
    ASTree* match_mulExpr();
    ASTree* match_plusExpr();
    ASTree* match_compareExpr();
    ASTree* match_judgeExpr();
    ASTree* match_landExpr();
    ASTree* match_lorExpr();
    ASTree* match_andExpr();
    ASTree* match_orExpr();
    ASTree* match_binaryExpr(func expr, int type[], int size);
    bool match_token(int type[], int size);
    bool isExprEnd(Token token);


private:
    std::vector<Token> m_tokens;
    Token m_current_token;
    ASTree* m_ast;
    int m_index;
    std::stack<Token> m_part;
};
#endif // _PARSER_H_
