#include "AST.h"

// ================= AstLeaf =================
AstLeaf::AstLeaf(Token token): m_token(token) {
}

// ================= AstList =================
AstList::AstList(std::vector<ASTree*> ast): m_ast(ast) {
}

std::string AstList::toString() {
    std::string str;
    str = "( ";
    for (std::vector<ASTree*>::iterator it = m_ast.begin(); it != m_ast.end(); ++it) {
        str += (*it)->toString() + ", ";
    }
    str += " )";
    return str;
}
int AstList::location() {
    int line = -1;
    for (std::vector<ASTree*>::iterator it = m_ast.begin(); it != m_ast.end(); ++it) {
        line = (*it)->location();
        if (line != -1) break;
    }
    return line;
}

// ================= IntNumberNode =================
IntNumberNode::IntNumberNode(Token token): AstLeaf(token) {
}

// ================= FloatNumberNode =================
FloatNumberNode::FloatNumberNode(Token token): AstLeaf(token) {
}

// ================= IdentifyNode =================
IdentifyNode::IdentifyNode(Token token): AstLeaf(token) {
}

// ================= UnitaryNode =================
UnitaryNode::UnitaryNode(Token operatorType, std::vector<ASTree*> ast): AstList(ast), m_operator(operatorType) {
}
std::string UnitaryNode::toString() {
    std::string str;
    str = "( ";
    str += m_operator.operator_toString();
    str += "( ";
    str += rightNode()->toString();
    str += " )";
    str += " )";
    return str;
}

// ================= BinaryNode =================
BinaryNode::BinaryNode(Token operatorType, std::vector<ASTree*> ast): AstList(ast), m_operator(operatorType) {
}
std::string BinaryNode::toString() {
    std::string str;
    str = "( ";
    //str += "( ";
    str += leftNode()->toString();
    //str += " )";
    str += m_operator.operator_toString();
    //str += "( ";
    str += rightNode()->toString();
    //str += " )";
    str += " )";
    return str;
}

// ================= BlockNode =================
BlockNode::BlockNode(std::vector<ASTree*> ast): AstList(ast) {
}
std::string BlockNode::toString() {
    std::string str;
    str = "( ";
    for (std::vector<ASTree*>::iterator it = m_ast.begin(); it != m_ast.end(); ++it) {
        str += (*it)->toString() + "; ";
    }
    str += " )";
    return str;
}

// ================= IfNode =================
IfNode::IfNode(std::vector<ASTree*> ast): AstList(ast) {
}
std::string IfNode::toString() {
    std::string str;
    str = "( if ";
    str += condition()->toString();
    str += body()->toString();
    if (elsebody() != NULL) {
        str += " else ";
        str += elsebody()->toString();
    }
    str += " )";
    return str;
}

// ================= WhileNode =================
WhileNode::WhileNode(std::vector<ASTree*> ast): AstList(ast) {
}
std::string WhileNode::toString() {
    std::string str;
    str = "( while ";
    str += condition()->toString();
    str += body()->toString();
    str += " )";
    return str;
}

// ================= ParamListNode =================
ParamsListNode::ParamsListNode(std::vector<ASTree*> ast): AstList(ast) {
}

// ================= DefNode =================
DefNode::DefNode(std::vector<ASTree*> ast): AstList(ast) {
}
std::string DefNode::toString() {
    std::string str;
    str = "( def ";
    str += name();
    if (paramlist() != NULL) {
        str += paramlist()->toString();
    }
    str += body()->toString();
    str += " )";
    return str;
}

// ================= ArgsNode =================
ArgsNode::ArgsNode(std::vector<ASTree*> ast): AstList(ast) {
}

// ================= CallNode =================
CallNode::CallNode(std::vector<ASTree*> ast): AstList(ast) {
}
std::string CallNode::toString() {
    std::string str;
    str = name();
    str += "( ";
    str += args()->toString();
    str += " )";
    return str;
}

