#include "AST.h"

// ================= AstLeaf =================
AstLeaf::AstLeaf(Token token): m_token(token) {
}

// ================= AstList =================
AstList::AstList(std::vector<ASTree*> ast): m_ast(ast) {
}

std::string AstList::toString() {
    std::string str;
    for (std::vector<ASTree*>::iterator it = m_ast.begin(); it != m_ast.end(); ++it) {
        str += "( " + (*it)->toString() + " )";
    }
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
    str += "( ";
    str += leftNode()->toString();
    str += " )";
    str += m_operator.operator_toString();
    str += "( ";
    str += rightNode()->toString();
    str += " )";
    str += " )";
    return str;
}

