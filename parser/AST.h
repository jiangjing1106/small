#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <vector>
#include "../lexer/Token.h"

class ASTree {
public:
   // virtual ASTree* child(i) = 0;
    virtual int numChildren() = 0;
    virtual std::string getObjectName() = 0;
    virtual std::string toString() = 0;
    virtual int location() = 0;
};

class AstLeaf : public ASTree {
public:
    AstLeaf(Token token);
   // virtual ASTree* child(i) { return this;}
    virtual int numChildren() {return 0;}
    virtual std::string getObjectName() = 0;
    virtual std::string toString() {return m_token.value_toString();}
    virtual Token token() {return m_token;}
    virtual int location() {return m_token.lineno;}
protected:
    Token m_token;
};

class AstList : public ASTree {
public:
    AstList(std::vector<ASTree*> ast);
  //  virtual ASTree* child(i) {return m_ast.at(i);}
    virtual int numChildren() {return m_ast.size();}
    virtual std::string getObjectName() = 0;
    virtual std::string toString();
    virtual int location();
protected:
    std::vector<ASTree*> m_ast;
};

class IntNumberNode : public AstLeaf {
public:
    IntNumberNode(Token token);
    int value() {return m_token.value.iValue;}
    virtual std::string getObjectName() {return "IntNumberNode";}
};

class FloatNumberNode : public AstLeaf {
public:
    FloatNumberNode(Token token);
    float value() {return m_token.value.fValue;}
    virtual std::string getObjectName() {return "FloatNumberNode";}
};

class IdentifyNode : public AstLeaf {
public:
    IdentifyNode(Token token);
    std::string value() {return m_token.value.sValue;}
    virtual std::string getObjectName() {return "IdentifyNode";}
};

class UnitaryNode : public AstList {
public:
    UnitaryNode(Token operatorType, std::vector<ASTree*> ast);
    ASTree* rightNode() {return m_ast.at(0);}
    Token operatorType() {return m_operator;}
    virtual std::string getObjectName() {return "UnitaryNode";}
    virtual std::string toString();
private:
    Token m_operator;
};

class BinaryNode : public AstList {
public:
    BinaryNode(Token operatorType, std::vector<ASTree*> ast);
    ASTree* leftNode() {return m_ast.at(0);}
    Token operatorType() {return m_operator;}
    ASTree* rightNode() {return m_ast.at(1);}
    virtual std::string getObjectName() {return "BinaryNode";}
    virtual std::string toString();
private:
    Token m_operator;
};
/*
class IfExpr : public AstList {
public:
    ASTree condition();
    ASTree body();
};

class WhileExpr : public AstList {
public:
    ASTree condition();
    ASTree body();
};

class FunctionExpr : public AstList {
public:
    ASTree paramlist();
    ASTree body();
};
*/
#endif // _AST_H_