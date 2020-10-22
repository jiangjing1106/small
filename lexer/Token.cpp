#include "Token.h"

std::string Token::toString() {
    std::string str;
    switch(type) {
        case TT_INT:
            str = "TT_INT:" + std::to_string(value.iValue);
            break;
        case TT_FLOAT:
            str = "TT_FLOAT:" + std::to_string(value.fValue);
            break;
        case TT_KEYWORD:
            str = "TT_KEYWORD:" + std::string(value.sValue);
            break;
        case TT_IDENTIFIER:
            str = "TT_IDENTIFIER:" + std::string(value.sValue);
            break;
        case TT_LP:
            str = "TT_LP";
            break;
        case TT_RP:
            str = "TT_RP";
            break;
        case TT_PLUS:
            str = "TT_PLUS";
            break;
        case TT_MINUS:
            str = "TT_MINUS";
            break;
        case TT_MUL:
            str = "TT_MUL";
            break;
        case TT_DIV:
            str = "TT_DIV";
            break;
        case TT_ASSIGN:
            str = "TT_ASSIGN";
            break;
        case TT_EQ:
            str = "TT_EQ";
            break;
        case TT_NEQ:
            str = "TT_NEQ";
            break;
        case TT_NOT:
            str = "TT_NOT";
            break;
        case TT_GT:
            str = "TT_GT";
            break;
        case TT_LT:
            str = "TT_LT";
            break;
        case TT_GTOREQ:
            str = "TT_GTOREQ";
            break;
        case TT_LTOREQ:
            str = "TT_LTOREQ";
            break;
        case TT_COMMA:
            str = "TT_COMMA";
            break;
        case TT_SEMICOLON:
            str = "TT_SEMICOLON";
            break;
        case TT_LB:
            str = "TT_LB";
            break;
        case TT_RB:
            str = "TT_RB";
            break;
        case TT_LAND:
            str = "TT_LAND";
            break;
        case TT_LOR:
            str = "TT_LOR";
            break;
        case TT_AND:
            str = "TT_AND";
            break;
        case TT_OR:
            str = "TT_OR";
            break;
        case TT_EOF:
            str = "TT_EOF";
            break;
        default:
            break;
    };

    return str;
}

std::string Token::operator_toString() {
    std::string str;
    switch(type) {
        case TT_LP:
            str = "(";
            break;
        case TT_RP:
            str = ")";
            break;
        case TT_PLUS:
            str = "+";
            break;
        case TT_MINUS:
            str = "-";
            break;
        case TT_MUL:
            str = "*";
            break;
        case TT_DIV:
            str = "/";
            break;
        case TT_ASSIGN:
            str = "=";
            break;
        case TT_EQ:
            str = "==";
            break;
        case TT_NEQ:
            str = "!=";
            break;
        case TT_NOT:
            str = "!";
            break;
        case TT_GT:
            str = ">";
            break;
        case TT_LT:
            str = "<";
            break;
        case TT_GTOREQ:
            str = ">=";
            break;
        case TT_LTOREQ:
            str = "<=";
            break;
        case TT_COMMA:
            str = ",";
            break;
        case TT_SEMICOLON:
            str = ";";
            break;
        case TT_LB:
            str = "{";
            break;
        case TT_RB:
            str = "}";
            break;
        case TT_LAND:
            str = "&";
            break;
        case TT_LOR:
            str = "!";
            break;
        case TT_AND:
            str = "&&";
            break;
        case TT_OR:
            str = "||";
            break;
        case TT_EOF:
            str = "eof";
            break;
        default:
            break;
    };

    return str;
}

std::string Token::value_toString() {
    std::string str;
    switch(type) {
        case TT_INT:
            str = std::to_string(value.iValue);
            break;
        case TT_FLOAT:
            str = std::to_string(value.fValue);
            break;
        case TT_KEYWORD:
        case TT_IDENTIFIER:
            str = std::string(value.sValue);
            break;
        default:
            break;
    };
    return str;
}
