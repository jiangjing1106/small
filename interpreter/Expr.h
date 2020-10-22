#ifndef _EXPR_H_
#define _EXPR_H_

#include "../AST.h"

class ASTreeEx : public ASTree {
public:
   virtual eval(Context cxt) = 0;
};

class AstLeafEx : public AstLeaf {
public:
    AstLeafEx(Token token);
    virtual eval(Context cxt) = 0;
protected:
    Token m_token;
};

class AstListEx : public AstList {
public:
    AstListEx(std::vector<ASTree*> ast);
    virtual eval(Context cxt) = 0;
protected:
    std::vector<ASTree*> m_ast;
};

class IntNumberNodeEx : public IntNumberNode {
public:
    IntNumberNodeEx(Token token);
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

#endif // _EXPR_H_
