#ifndef _LEXER_H_
#define _LEXER_H_

#include <vector>
#include <string>
#include <fstream>
#include "Token.h"

class Lexer {

public:
    Lexer(char* file);
    ~Lexer();

    std::vector<Token> make_tokens();
    void print_tokens();

private:
    void openFile();
    void closeFile();
    void advance();
    bool isDigit(char c);
    bool isLetter(char c);
    bool isSpace(char c);
    std::string toString(Token token);

    Token make_numbers();
    Token make_identifier();
    Token make_assign_or_eq();
    Token make_not_or_neq();
    Token make_gt_or_gtoreq();
    Token make_lt_or_ltoreq();
    Token make_land_or_and();
    Token make_lor_or_or();

private:
    char* m_file;
    std::ifstream m_ifs;
    std::vector<Token> m_tokens;
    char m_current_char;
    int m_current_lnno;
    int m_current_colno;
};

#endif // _LEXER_H_
