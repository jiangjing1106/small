#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Lexer.h"

static char* KEYWORDS[] = {
    "if",
    "else",
    "elif",
    "while",
    "var",
    "def",
};

Lexer::Lexer(char* file) {
    m_file = file;
    m_current_char = ' ';
    m_current_lnno = 0;
    m_current_colno = 0;
    openFile();
}

Lexer::~Lexer() {

}

/*
** 打开源文件
*/
void Lexer::openFile() {
    m_ifs.open(m_file, std::ifstream::in);
}

/*
** 关闭源文件
*/
void Lexer::closeFile() {
    m_ifs.close();
}

/*
** 获取下一个字符
*/
void Lexer::advance() {
     m_current_colno++;
    if (!m_ifs.eof()) {
        m_current_char = m_ifs.get();
        printf("c is %s\n", &m_current_char);
        if (m_current_char == EOF) {
            printf("eof \n");
            Token token;
            token.type = TT_EOF;
            token.lineno = m_current_lnno;
            m_tokens.push_back(token);
            closeFile();
        }
    }
}

bool Lexer::isDigit(char c) {
    if (c >= '0' && c <= '9') {
        return true;
    } else {
        return false;
    }
}

bool Lexer::isLetter(char c) {
    if ((c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')) {
        return true;
    } else {
        return false;
    }
    
}

bool Lexer::isSpace(char c) {
    if (c == '\t' || c == '\n' || c == ' ') {
        return true;
    } else {
        return false;
    }
}

/*
** 获取数字类型的token
*/
Token Lexer::make_numbers() {
    Token token;
    int dot_count = 0;
    std::string str;

    while (isDigit(m_current_char) || m_current_char == '.') {
        if (m_current_char == '.') {
            // 已经存在小数点了
            if (dot_count == 1) throw "dot is already exist";

            dot_count++;
        }
        str.append(&m_current_char);
        advance();
    }

    //printf("make_numbers is %s\n", str.c_str());
    if (dot_count == 0) {
        token.type = TT_INT;
        token.lineno = m_current_lnno;
        token.value.iValue = std::stoi(str);
    } else {
        token.type = TT_FLOAT;
        token.lineno = m_current_lnno;
        token.value.fValue = std::stof(str);
    }

    return token;
}

/*
** 获取标志符或关键字类型的token
*/
Token Lexer::make_identifier() {
    Token token;
    std::string str;

    do {
        str.append(&m_current_char);
        advance();
    } while (isLetter(m_current_char) || isDigit(m_current_char) || m_current_char == '_');

    //printf("make_identifier is %s\n", str.c_str());
    strncpy(token.value.sValue, str.c_str(), str.length());
    token.value.sValue[str.length()] = '\0';

    for(int i=0; i < sizeof(KEYWORDS)/sizeof(KEYWORDS[0]); i++) {
        if (strcmp(KEYWORDS[i], str.c_str()) == 0) {
            token.type = TT_KEYWORD;
            token.lineno = m_current_lnno;
            //printf("make_keyword is %s\n", token.value.sValue);
            return token;
        }
    }

    token.type = TT_IDENTIFIER;
    token.lineno = m_current_lnno;

    return token;
}

/*
** 获取'='或'=='类型的token
*/
Token Lexer::make_assign_or_eq() {
    Token token;

    advance();
    if (m_current_char == '=') {
        token.type = TT_EQ;
        advance();
    } else {
        token.type = TT_ASSIGN;
    }

    return token;
}

/*
** 获取'!'或'!='类型的token
*/
Token Lexer::make_not_or_neq() {
    Token token;

    advance();
    if (m_current_char == '=') {
        token.type = TT_NEQ;
        token.lineno = m_current_lnno;
        advance();
    } else {
        token.type = TT_NOT;
        token.lineno = m_current_lnno;
    }

    return token;
}

/*
** 获取'>'或'>='类型的token
*/
Token Lexer::make_gt_or_gtoreq() {
    Token token;

    advance();
    if (m_current_char == '=') {
        token.type = TT_GTOREQ;
        token.lineno = m_current_lnno;
        advance();
    } else {
        token.type = TT_GT;
        token.lineno = m_current_lnno;
    }

    return token;
}

/*
** 获取'<'或'<='类型的token
*/
Token Lexer::make_lt_or_ltoreq() {
    Token token;

    advance();
    if (m_current_char == '=') {
        token.type = TT_LTOREQ;
        token.lineno = m_current_lnno;
        advance();
    } else {
        token.type = TT_LT;
        token.lineno = m_current_lnno;
    }

    return token;
}

/*
** 获取'&'或'&&'类型的token
*/
Token Lexer::make_land_or_and() {
    Token token;

    advance();
    if (m_current_char == '&') {
        token.type = TT_AND;
        token.lineno = m_current_lnno;
        advance();
    } else {
        token.type = TT_LAND;
        token.lineno = m_current_lnno;
    }

    return token;
}

/*
** 获取'|'或'||'类型的token
*/
Token Lexer::make_lor_or_or() {
    Token token;

    advance();
    if (m_current_char == '|') {
        token.type = TT_OR;
        token.lineno = m_current_lnno;
        advance();
    } else {
        token.type = TT_LOR;
        token.lineno = m_current_lnno;
    }

    return token;
}

/*
** 遍历文件，获取token列表
*/
std::vector<Token> Lexer::make_tokens() {
    try {
        m_current_lnno++;
        advance();
        while (m_current_char != EOF) {
            if (isSpace(m_current_char)) {
                if (m_current_char == '\n') {
                    m_current_lnno++;
                    m_current_colno = 0;
                }
                advance();
                continue;
            }

            if (isDigit(m_current_char)) {
                m_tokens.push_back(make_numbers());
            } else if (isLetter(m_current_char) || m_current_char == '_') {
                m_tokens.push_back(make_identifier());
            } else if (m_current_char == '(') {
                Token token;
                token.type = TT_LP;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == ')') {
                Token token;
                token.type = TT_RP;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == '+') {
                Token token;
                token.type = TT_PLUS;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == '-') {
                Token token;
                token.type = TT_MINUS;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == '*') {
                Token token;
                token.type = TT_MUL;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == '/') {
                Token token;
                token.type = TT_DIV;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == '=') {
                m_tokens.push_back(make_assign_or_eq());
            } else if (m_current_char == '!') {
                m_tokens.push_back(make_not_or_neq());
            } else if (m_current_char == '>') {
                m_tokens.push_back(make_gt_or_gtoreq());
            } else if (m_current_char == '<') {
                m_tokens.push_back(make_lt_or_ltoreq());
            } else if (m_current_char == ',') {
                Token token;
                token.type = TT_COMMA;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == ';') {
                Token token;
                token.type = TT_SEMICOLON;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == '{') {
                Token token;
                token.type = TT_LB;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == '}') {
                Token token;
                token.type = TT_RB;
                token.lineno = m_current_lnno;
                m_tokens.push_back(token);
                advance();
            } else if (m_current_char == '&') {
                m_tokens.push_back(make_land_or_and());
            } else if (m_current_char == '|') {
                m_tokens.push_back(make_lor_or_or());
            } else {
                throw "Illegal char";
            }

        }
    } catch (const char* msg) {
        printf("%s:%d:%d error: %c is %s\n", m_file, m_current_lnno, m_current_colno, m_current_char, msg);
        exit(1);
    }

    return m_tokens;
}

void Lexer::print_tokens() {
    for (unsigned i=0; i<m_tokens.size(); i++) {
        printf("token is [%s]\n", m_tokens.at(i).toString().c_str());
    }
};

