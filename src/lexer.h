#pragma once
#include <string>

enum class TokType {
    IDENT,
    NUMBER,
    KW_PRINT,

    PLUS,
    STAR,
    EQUAL,
    LPAREN,
    RPAREN,
    SEMI,

    END
};

struct Token {
    TokType type;
    std::string lexeme;   // original text like "x", "123", "+", etc.
    long long number = 0; // only used if type == NUMBER
};

std::string tokTypeToString(TokType t);

class Lexer {
public:
    explicit Lexer(std::string source);

    Token next(); // get next token

private:
    std::string src;
    size_t i = 0;

    void skipWhitespace();
    static bool isIdentStart(char c);
    static bool isIdentChar(char c);
};
