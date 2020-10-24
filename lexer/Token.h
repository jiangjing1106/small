#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

typedef enum TOKEN_e {
    TT_INT        = 0,
    TT_FLOAT      = 1,
    TT_KEYWORD    = 2,
    TT_IDENTIFIER = 3,
    TT_LP         = 4,     // '('
    TT_RP         = 5,     // ')'
    TT_PLUS       = 6,     // '+'
    TT_MINUS      = 7,     // '-'
    TT_MUL        = 8,     // '*'
    TT_DIV        = 9,     // '/'
    TT_ASSIGN     = 10,    // '='
    TT_EQ         = 11,    // '=='
    TT_NEQ        = 12,    // '!='
    TT_NOT        = 13,    // '!'
    TT_GT         = 14,    // '>'
    TT_LT         = 15,    // '<'
    TT_GTOREQ     = 16,    // '>='
    TT_LTOREQ     = 17,    // '<='
    TT_COMMA      = 18,    // ','
    TT_SEMICOLON  = 19,    // ';'
    TT_LB         = 20,    // '{'
    TT_RB         = 21,    // '}' 
    TT_LAND       = 22,    // '&'
    TT_LOR        = 23,    // '|'
    TT_AND        = 24,    // '&&'
    TT_OR         = 25,    // '||'
    TT_EOF        = 26,
} TOKEN_TYPE;

typedef struct Token_t {
    TOKEN_TYPE type;
    union {
        int iValue;
        float fValue;
        char sValue[32];
    } value;
    int lineno;
    std::string toString();
    std::string operator_toString();
    std::string value_toString();
} Token;

#endif // _TOKEN_H_
