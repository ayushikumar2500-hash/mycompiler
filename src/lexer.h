#pragma once
#include <string>

enum class TokType {
    IDENT,
    NUMBER,
    KW_PRINT,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    EQUAL,
    LPAREN,
    RPAREN,
    SEMI,

    END
};

struct Token {
    TokType type;
    std::string lexeme;
    long long number = 0;
};

std::string tokTypeToString(TokType t);

class Lexer {
public:
    explicit Lexer(std::string source);
    Token next();

private:
    std::string src;
    size_t i = 0;

    void skipWhitespace();
    static bool isIdentStart(char c);
    static bool isIdentChar(char c);
};
