#include "Parser.h"

#include <iostream>
#include <typeinfo>

#define SUCCESS  0
#define FAILED  -1

//Token Parser::m_tokens = std::vector<Token_t>();
//int Parser::m_index = -1;

Parser::Parser(std::vector<Token> tokens) {
    m_tokens = tokens;
    m_index = -1;
}

Parser::~Parser() {
}

void Parser::advance() {
    try {
        m_index++;
        if (m_index < m_tokens.size()) {
            m_current_token = m_tokens.at(m_index);
            //printf("advance token is %s\n", m_current_token.toString().c_str());
            if (m_current_token.type == TT_RP) {
                if (!m_part.empty()) {
                    m_part.pop();
                    advance();
                } else {
                    throw "'(' and ')' are not matched";
                }
            } else if (m_current_token.type == TT_LP) {
                m_part.push(m_current_token);
            }
        }
    } catch (const char* msg) {
        printf("%d error: Syntax error:  %s\n", m_current_token.lineno, msg);
        exit(1);
    }
}

void Parser::fallback() {
    m_index--;
    if (m_index < m_tokens.size()) {
        m_current_token = m_tokens.at(m_index);
        //printf("fallback token is %s\n", m_current_token.toString().c_str());
    }
}

ASTree* Parser::make_ast() {
    try {
        while (m_current_token.type != TT_EOF) {
            advance();
            m_ast = match_operatorExpr();
            printf("m_ast is %s\n", m_ast->toString().c_str());
            if (!m_part.empty()) throw "'(' and ')' are not matched";
        }
        return m_ast;
    } catch (const char* msg) {
        printf("%d error: Syntax error:  %s\n", m_current_token.lineno, msg);
        exit(1);
    }
}

/*
primary : "(" expr ")" | TT_INT | TT_FLOAT | TT_IDENTIFIER
factor : ("-" | "+" ) primary | primary
expr : factor { OP factor }
block : "(" [ statement ] { (";" | EOL) [ statement ] } "}"
simple : expr
statement : "if" expr block [ {"elif" expr block} "else" block ]
          | "while" expr block
          | simple
program : [ statement ] (";" | EOL)


param      : TT_IDENTIFIER
params     : param { "," param }
param_list : "(" [ param] ")"
def        : "def" TT_IDENTIFIER param_list block
block      : "(" [ statement ] { (";" | EOL) [ statement ] } "}"
args       : expr { "," expr }
postfix    : "(" [ args ] ")"
primary    : "(" expr ")" | TT_INT | TT_FLOAT | TT_IDENTIFIER | { postfix }
factor     : ("-" | "+" ) primary | primary
expr       : factor { OP factor }
simple     : expr [ args ]
statement  : "if" expr block [ {"elif" expr block} "else" block ]
           | "while" expr block
           | simple
program    : [ def | statement ] ( ";" | EOL)





*/

ASTree* Parser::match_program() {

    ProgramStmnt note = ProgramStmnt
    
}

ASTree* Parser::match_statement() {
    if (m_current_token.type == TT_KEYWORD) {
        if (!strcmp(m_current_token.value.sValue, "if")) {
            std::vector<ASTree*> ast;
            ASTree* if_condition = match_orExpr();
            ast.push_back(if_condition);
            ASTree* if_body = match_block();
            ast.push_back(if_body);
            while
            if ()
            IfNode* node = new IfNode(ast);
            return node;
        } else if (!strcmp(m_current_token.value.sValue, "while")) {
            std::vector<ASTree*> ast;
            ASTree* while_condition = match_orExpr();
            ast.push_back(while_condition);
            ASTree* while_body = match_block();
            ast.push_back(while_body);
            WhileNode* node = new WhileNode(ast);
            return node;
        }
    } else {
    }
}




/*
** operatorExpr -> orExpr
*/
ASTree* Parser::match_operatorExpr() {
    return match_orExpr();
}

/*
** factor -> TT_INT | TT_FLOAT | TT_IDENTIFIER
**        -> TT_LP expr TT_RP
*/
ASTree* Parser::match_factor() {
    try {
        if (m_current_token.type == TT_INT) {
            IntNumberNode* node = new IntNumberNode(m_current_token);
            return node;
        } else if (m_current_token.type == TT_FLOAT) {
            FloatNumberNode* node = new FloatNumberNode(m_current_token);
            return node;
        } else if (m_current_token.type == TT_IDENTIFIER) {
            IdentifyNode* node = new IdentifyNode(m_current_token);
            return node;
        } else if (m_current_token.type == TT_LP) {
            advance();
            if (isExprEnd(TT_SEMICOLON)) {
                throw "Expect ')' or expr after ')'";
            }
            ASTree* node = match_operatorExpr();
            if (isExprEnd(TT_SEMICOLON)) {
                return node;
            }
            advance();
            return node;        
        } else {
                throw "Illegal Syntax";
        }
    } catch (const char* msg) {
        printf("%d error: Syntax error: %s is %s\n", m_current_token.lineno, m_current_token.toString().c_str(), msg);
        exit(1);
    }
}

/*
** unitaryExpr -> [ (TT_PLUS | TT_MINUS | TT_NOT) ] factor
*/
ASTree* Parser::match_unitaryExpr() {
    try {
        if (m_current_token.type == TT_PLUS || m_current_token.type == TT_MINUS || m_current_token.type == TT_NOT) {
            Token operatorType = m_current_token;
            advance();
            if (isExprEnd(TT_SEMICOLON)) {
                throw "Expect a number or expr after a operator";
            }
            std::vector<ASTree*> ast;
            ASTree* right_node = match_factor();
            ast.push_back(right_node);
            UnitaryNode* node = new UnitaryNode(operatorType, ast);
            return node;
        } else {
            return match_factor();
        }
    } catch (const char* msg) {
        printf("%d error: Syntax error:  %s\n", m_current_token.lineno, msg);
        exit(1);
    }
}

/*
** mulExpr -> unitaryExpr ((T_MUL | TT_DIV) unitaryExpr)*
*/
ASTree* Parser::match_mulExpr() {
    int type[2] = {TT_MUL, TT_DIV};
    return match_binaryExpr(&Parser::match_unitaryExpr, type, (int)(sizeof(type)/sizeof(int)));
}

/*
** plusExpr -> mulExpr ((TT_PLUS | TT_MINUS) mulExpr)*
*/
ASTree* Parser::match_plusExpr() {
    int type[2] = {TT_PLUS, TT_MINUS};
    return match_binaryExpr(&Parser::match_mulExpr, type, (int)(sizeof(type)/sizeof(int)));
}

/*
** compareExpr -> plusExpr ((TT_GT | TT_LT | TT_GTOREQ | TT_LTOREQ) plusExpr)*
*/
ASTree* Parser::match_compareExpr() {
    int type[4] = {TT_GT, TT_LT, TT_GTOREQ, TT_LTOREQ};
    return match_binaryExpr(&Parser::match_plusExpr, type, (int)(sizeof(type)/sizeof(int)));
}

/*
** judgeExpr -> compareExpr ((TT_EQ | TT_NEQ ) compareExpr)*
*/
ASTree* Parser::match_judgeExpr() {
    int type[2] = {TT_EQ, TT_NEQ};
    return match_binaryExpr(&Parser::match_compareExpr, type, (int)(sizeof(type)/sizeof(int)));
}

/*
** landExpr -> judgeExpr (TT_LAND judgeExpr)*
*/
ASTree* Parser::match_landExpr() {
    int type[1] = {TT_LAND};
    return match_binaryExpr(&Parser::match_judgeExpr, type, (int)(sizeof(type)/sizeof(int)));
}

/*
** lorExpr -> landExpr (TT_LOR landExpr)*
*/
ASTree* Parser::match_lorExpr() {
    int type[1] = {TT_LOR};
    return match_binaryExpr(&Parser::match_landExpr, type, (int)(sizeof(type)/sizeof(int)));
}

/*
** andExpr -> lorExpr (TT_AND lorExpr)*
*/
ASTree* Parser::match_andExpr() {
    int type[1] = {TT_AND};
    return match_binaryExpr(&Parser::match_lorExpr, type, (int)(sizeof(type)/sizeof(int)));
}

/*
** orExpr -> andExpr (TT_OR andExpr)*
*/
ASTree* Parser::match_orExpr() {
    int type[1] = {TT_OR};
    return match_binaryExpr(&Parser::match_andExpr, type, (int)(sizeof(type)/sizeof(int)));
}

ASTree* Parser::match_binaryExpr(func expr, int type[], int size) {
    try {
        //printf("match_binaryExpr %d\n", type[0]);
        std::vector<ASTree*> ast;
        ASTree* leftNode = (this->*expr)();
        ast.push_back(leftNode);
        advance();
        if (isExprEnd(TT_SEMICOLON)) return leftNode;
        if (!match_token(type, size)) {
            fallback();
            return leftNode;
        }

        Token operatorType;
        operatorType = m_current_token;
        advance();
        if (isExprEnd(TT_SEMICOLON)) {
            throw "Expect a number or expr after a operator";
        }
        ASTree* rightNode = (this->*expr)();
        ast.push_back(rightNode);
        BinaryNode* node = new BinaryNode(operatorType, ast);
        advance();
        if (isExprEnd(TT_SEMICOLON)) return node;
        if (!match_token(type, size)) {
            fallback();
            return node;
        }
        do {
            std::vector<ASTree*> ast_child;
            leftNode = node;
            ast_child.push_back(leftNode);
            operatorType = m_current_token;
            advance();
            if (isExprEnd(TT_SEMICOLON)) {
                throw "Expect a number or expr after a operator";
            }
            rightNode = (this->*expr)();
            ast_child.push_back(rightNode);
            node = new BinaryNode(operatorType, ast_child);
            if (isExprEnd(TT_SEMICOLON)) {
                return node;
            } else {
                advance();
                if (isExprEnd(TT_SEMICOLON)) return node;
            }
        } while (match_token(type, size));
        fallback();
        return node;
    } catch (const char* msg) {
        printf("%d error: Syntax error:  %s\n", m_current_token.lineno, msg);
        exit(1);
    }
}

bool Parser::match_token(int type[], int size) {
    bool match = false;
    for (int i= 0; i < size; i++) {
        //printf("match_token m_current_token.type is %d, type is %d\n", m_current_token.type, type[i]);
        if (m_current_token.type == type[i]) {
            match = true;
            break;
        }
    }
    return match;
}

bool Parser::isExprEnd(Token token) {
    if (m_current_token.type == TT_EOF || m_current_token.type == token.type) {
        return true;
    } else {
        return false;
    }
}


