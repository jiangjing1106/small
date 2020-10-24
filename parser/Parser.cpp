#include <string.h>
#include <iostream>
#include <typeinfo>

#include "Parser.h"

Parser::Parser(char* file, std::vector<Token> tokens, SymbolTable* var, SymbolTable* func) {
    m_file = file;
    m_tokens = tokens;
    m_index = -1;
    m_var_table = var;
    m_func_table = func;
    m_brace_num = 0;
}

Parser::~Parser() {
}

void Parser::advance() {
    m_index++;
    if (m_index < m_tokens.size()) {
        m_current_token = m_tokens.at(m_index);
        //printf("advance token is %s\n", m_current_token.toString().c_str());
    }
}

void Parser::fallback() {
    m_index--;
    if (m_index < m_tokens.size()) {
        m_current_token = m_tokens.at(m_index);
        //printf("fallback token is %s\n", m_current_token.toString().c_str());
    }
}

void Parser::match_program() {
    try {
        advance();
        while (m_current_token.type != TT_EOF) {
        printf("%s, %d, %s\n", __func__, __LINE__, m_current_token.toString().c_str());
            if (m_current_token.type == TT_KEYWORD && !strcmp(m_current_token.value.sValue, "def")) {
                advance();
                DefNode* def = (DefNode*)match_defstmmt();
                m_func_table->put(def->name(), def);
            } else if (m_current_token.type == TT_IDENTIFIER) {
                //advance();
                //IdentifyNode* var = (IdentifyNode*)match_statement();
              //  m_var_table->put(var->value(), var);
                BinaryNode* var = (BinaryNode*)match_assignExpr();
                m_var_table->put(((IdentifyNode*)(var->leftNode()))->value(), var);
                printf("var is %s\n", var->toString().c_str());
                advance();
            } else {
                throw "Illegal Syntax";
            }
        printf("%s, %d, %s\n", __func__, __LINE__, m_current_token.toString().c_str());
        }
    } catch (const char* msg) {
        printf("%s:%d error: Syntax error: %s is %s\n", m_file, m_current_token.lineno, m_current_token.toString().c_str(), msg);
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
param_list : "(" [ params] ")"
def        : "def" TT_IDENTIFIER param_list block
block      : "{" [ statement ] { (";" | EOL) [ statement ] } "}"
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

/*
** defstmmt
*/
ASTree* Parser::match_defstmmt() {
    try {
        printf("%s, %d\n", __func__, __LINE__);
        if (m_current_token.type != TT_IDENTIFIER) throw "Except IDENTIFIER";
        std::vector<ASTree*> ast;
        IdentifyNode* name = new IdentifyNode(m_current_token);
        printf("%s, %d, %s\n", __func__, __LINE__, name->toString().c_str());
        ast.push_back(name);
        advance();
        ASTree* paramlist = match_paramlist();
        ast.push_back(paramlist);
        printf("%s, %d, %s\n", __func__, __LINE__, m_current_token.toString().c_str());
        advance();
        ASTree* body = match_blockstmmt();
        ast.push_back(body);
        DefNode* node = new DefNode(ast);
        printf("%s, %d, DefNode is %s\n", __func__, __LINE__, node->toString().c_str());
        return node;
    } catch (const char* msg) {
        printf("%d error: Syntax error: %s is %s\n", m_current_token.lineno, m_current_token.toString().c_str(), msg);
        exit(1);
    }
}

/*
** paramlist
*/
ASTree* Parser::match_paramlist() {
    try {
        printf("%s, %d\n", __func__, __LINE__);
        if (m_current_token.type != TT_LP) throw "Except '('";
        advance();
        if (m_current_token.type == TT_RP) {
            return NULL; // no param
        }
        if (m_current_token.type != TT_IDENTIFIER) {
            throw "Except 'IDENTIFIER'";
        }

        std::vector<ASTree*> ast;
        IdentifyNode* name = new IdentifyNode(m_current_token);
        ast.push_back(name);
        advance();
        while (m_current_token.type == TT_COMMA) {
            advance();
            if (m_current_token.type != TT_IDENTIFIER) {
                throw "Except 'IDENTIFIER'";
            } else {
                name = new IdentifyNode(m_current_token);
                ast.push_back(name);
                advance();
            }
        }
        
        if (m_current_token.type != TT_RP) throw "Except ')'";

        ParamsListNode* node = new ParamsListNode(ast);
        printf("%s, %d, paramlist is %s\n", __func__, __LINE__, node->toString().c_str());
        return node;
    } catch (const char* msg) {
        printf("%d error: Syntax error: %s is %s\n", m_current_token.lineno, m_current_token.toString().c_str(), msg);
        exit(1);
    }
}

/*
** ifstmmt
*/
ASTree* Parser::match_ifstmmt() {
    std::vector<ASTree*> ast;
    ASTree* if_condition = match_operatorExpr();
    ast.push_back(if_condition);
    printf("%s, %d, if_condition is %s\n", __func__, __LINE__, if_condition->toString().c_str());
    advance();
    ASTree* if_body = match_blockstmmt();
    printf("%s, %d, if_body is %s\n", __func__, __LINE__, if_body->toString().c_str());
    ast.push_back(if_body);
    advance();
    if (!strcmp(m_current_token.value.sValue, "else")) {
        advance();
        ASTree* else_body = match_blockstmmt();
        printf("%s, %d, else_body is %s\n", __func__, __LINE__, else_body->toString().c_str());
        ast.push_back(else_body);  
        advance();
    }
    IfNode* node = new IfNode(ast);
    printf("%s, %d, IfNode is %s\n", __func__, __LINE__, node->toString().c_str());
    return node;
}

/*
** whilestmmt
*/
ASTree* Parser::match_whilestmmt() {
    std::vector<ASTree*> ast;
    ASTree* while_condition = match_operatorExpr();
    ast.push_back(while_condition);
    advance();
    ASTree* while_body = match_blockstmmt();
    ast.push_back(while_body);
    WhileNode* node = new WhileNode(ast);
    printf("%s, %d, WhileNode is %s\n", __func__, __LINE__, node->toString().c_str());
    return node;
}

/*
** callstmt
*/
ASTree* Parser::match_callstmt(Token var) {
    std::vector<ASTree*> ast;
    IdentifyNode* id = new IdentifyNode(var);
    ast.push_back(id);
    std::vector<ASTree*> args;
    while(m_current_token.type != TT_RP) {
        advance();
        args.push_back(match_operatorExpr());
        advance();
        if (m_current_token.type != TT_COMMA) break;
    }
    ArgsNode* argsNode = new ArgsNode(args);
    ast.push_back(argsNode);
    CallNode* node = new CallNode(ast);
    printf("%s, %d, CallNode is %s\n", __func__, __LINE__, node->toString().c_str());
    return node;
}

/*
** blockstmmt
*/
ASTree* Parser::match_blockstmmt() {
    try {
        printf("%s, %d\n", __func__, __LINE__);
        if (m_current_token.type != TT_LB) {
            throw "Except '{' here";
        } else {
            m_brace_num += 1;
            printf("m_brace_num + 1 %d\n", m_brace_num);
        }
        advance();
        printf("%s, %d, %s\n", __func__, __LINE__, m_current_token.toString().c_str());
        std::vector<ASTree*> ast;
        while (m_brace_num > 0 && m_current_token.type != TT_EOF) {
            if (m_current_token.type == TT_KEYWORD) {
                if (!strcmp(m_current_token.value.sValue, "if")) {
                    advance();
                    ast.push_back(match_ifstmmt());
                } else if (!strcmp(m_current_token.value.sValue, "while")) {
                    advance();
                    ast.push_back(match_whilestmmt());
                } else {
                    throw "Illegal Syntax";
                }
            } else if (m_current_token.type == TT_RB) {
                m_brace_num -= 1;
                printf("m_brace_num - 1 %d\n", m_brace_num);
            } else if (m_current_token.type == TT_IDENTIFIER) {
                Token var = m_current_token;
                advance();
                if (m_current_token.type == TT_LP) {
                    ast.push_back(match_callstmt(var));
                } else {
                    fallback();
                    ast.push_back(match_operatorExpr());
                }
                printf("%s, %d, %s\n", __func__, __LINE__, m_current_token.toString().c_str());
            }
            advance();
        printf("%s, %d, %s, m_brace_num is %d\n", __func__, __LINE__, m_current_token.toString().c_str(), m_brace_num);
        }
        printf("%s, %d, %s\n", __func__, __LINE__, m_current_token.toString().c_str());
        if (m_brace_num != 0) throw "'{' and '}' are not matched";
        if (m_brace_num == 0) fallback();
        BlockNode* node = new BlockNode(ast);
        printf("%s, %d, BlockNode is %s\n", __func__, __LINE__, node->toString().c_str());
        return node;
    } catch (const char* msg) {
        printf("%d error: Syntax error: %s is %s\n", m_current_token.lineno, m_current_token.toString().c_str(), msg);
        exit(1);
    }
}

/*
** operatorExpr -> orExpr
*/
ASTree* Parser::match_operatorExpr() {
    return match_assignExpr();
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
            m_part.push(m_current_token);
            advance();
            ASTree* node = match_operatorExpr();
            advance();
            bool haveRP = false;
            while (m_current_token.type == TT_RP) {
                haveRP = true;
                m_part.pop();
                advance();
            }
            if (haveRP) {
                haveRP = false;
                fallback();
            }

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
            if (isExprEnd(m_current_token)) {
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
        printf("%d error: Syntax error: %s is %s\n", m_current_token.lineno, m_current_token.toString().c_str(), msg);
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

/*
** assignExpr -> factor TT_ASSIGN orExpr
*/
ASTree* Parser::match_assignExpr() {
    int type[1] = {TT_ASSIGN};
    return match_binaryExpr(&Parser::match_orExpr, type, (int)(sizeof(type)/sizeof(int)));
}


ASTree* Parser::match_binaryExpr(func expr, int type[], int size) {
    try {
        //printf("match_binaryExpr %s\n", m_current_token.toString().c_str());
        std::vector<ASTree*> ast;
        ASTree* leftNode = (this->*expr)();
        //printf("match_binaryExpr leftNode %s\n", leftNode->toString().c_str());
        ast.push_back(leftNode);
        advance();
        if (m_current_token.type == TT_ASSIGN && leftNode->getObjectName().compare("IdentifyNode")) {
            throw "lvalue required as left operand of assignment";
        }
        if (isExprEnd(m_current_token)) return leftNode;
        if (!match_token(type, size)) {
            fallback();
            return leftNode;
        }

        Token operatorType;
        operatorType = m_current_token;
        advance();
        if (isExprEnd(m_current_token)) {
            throw "Expect a number or expr after a operator";
        }
        ASTree* rightNode = (this->*expr)();
        ast.push_back(rightNode);
        BinaryNode* node = new BinaryNode(operatorType, ast);
        advance();
        if (isExprEnd(m_current_token)) return node;
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
            if (isExprEnd(m_current_token)) {
                throw "Expect a number or expr after a operator";
            }
            rightNode = (this->*expr)();
            ast_child.push_back(rightNode);
            node = new BinaryNode(operatorType, ast_child);
            if (isExprEnd(m_current_token)) {
                return node;
            } else {
                advance();
                if (isExprEnd(m_current_token)) return node;
            }
        } while (match_token(type, size));
        fallback();
        return node;
    } catch (const char* msg) {
        printf("%d error: Syntax error: %s is %s\n", m_current_token.lineno, m_current_token.toString().c_str(), msg);
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
    if (token.type == TT_EOF || token.type == TT_SEMICOLON) {
        return true;
    } else {
        return false;
    }
}


