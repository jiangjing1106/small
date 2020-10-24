#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <stack>

#include "../lexer/Token.h"
#include "AST.h"
#include "SymbolTable.h"

class Parser {
public:
    Parser(char* file, std::vector<Token> tokens, SymbolTable* var, SymbolTable* func);
    ~Parser();

    void match_program();

private:

    typedef ASTree* (Parser::*func)();

    void advance();
    void fallback();
    ASTree* match_defstmmt();
    ASTree* match_paramlist();
    ASTree* match_ifstmmt();
    ASTree* match_whilestmmt();
    ASTree* match_blockstmmt();
    ASTree* match_callstmt(Token var);
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
    ASTree* match_assignExpr();
    ASTree* match_binaryExpr(func expr, int type[], int size);
    bool match_token(int type[], int size);
    bool isExprEnd(Token token);


private:
    char* m_file;
    std::vector<Token> m_tokens;
    Token m_current_token;
    ASTree* m_ast;
    int m_index;
    std::stack<Token> m_part;
    std::string m_current_stmt;
    int m_brace_num;
    SymbolTable* m_var_table;
    SymbolTable* m_func_table;
};
#endif // _PARSER_H_
