#include "lexer.h"
#include <cctype>
#include <stdexcept>

using std::string;

static std::runtime_error lexError(const string& msg) {
    return std::runtime_error("Lex error: " + msg);
}

std::string tokTypeToString(TokType t) {
    switch (t) {
        case TokType::IDENT:    return "IDENT";
        case TokType::NUMBER:   return "NUMBER";
        case TokType::KW_PRINT: return "KW_PRINT";

        case TokType::PLUS:     return "PLUS";
        case TokType::MINUS:    return "MINUS";
        case TokType::STAR:     return "STAR";
        case TokType::SLASH:    return "SLASH";

        case TokType::EQUAL:    return "EQUAL";
        case TokType::LPAREN:   return "LPAREN";
        case TokType::RPAREN:   return "RPAREN";
        case TokType::SEMI:     return "SEMI";

        case TokType::END:      return "END";
    }
    return "UNKNOWN";
}

Lexer::Lexer(std::string source) : src(std::move(source)) {}

bool Lexer::isIdentStart(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return std::isalpha(uc) || c == '_';
}

bool Lexer::isIdentChar(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return std::isalnum(uc) || c == '_';
}

void Lexer::skipWhitespace() {
    while (i < src.size()) {
        unsigned char uc = static_cast<unsigned char>(src[i]);
        if (!std::isspace(uc)) break;
        i++;
    }
}

Token Lexer::next() {
    skipWhitespace();

    if (i >= src.size()) {
        return Token{TokType::END, ""};
    }

    char c = src[i];

    // Single-character tokens
    if (c == '+') { i++; return Token{TokType::PLUS,  "+"}; }
    if (c == '-') { i++; return Token{TokType::MINUS, "-"}; }
    if (c == '*') { i++; return Token{TokType::STAR,  "*"}; }
    if (c == '/') { i++; return Token{TokType::SLASH, "/"}; }

    if (c == '=') { i++; return Token{TokType::EQUAL,  "="}; }
    if (c == '(') { i++; return Token{TokType::LPAREN, "("}; }
    if (c == ')') { i++; return Token{TokType::RPAREN, ")"}; }
    if (c == ';') { i++; return Token{TokType::SEMI,   ";"}; }

    // Number: [0-9]+
    {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isdigit(uc)) {
            size_t start = i;
            while (i < src.size()) {
                unsigned char uc2 = static_cast<unsigned char>(src[i]);
                if (!std::isdigit(uc2)) break;
                i++;
            }
            string lex = src.substr(start, i - start);
            long long val = std::stoll(lex);
            return Token{TokType::NUMBER, lex, val};
        }
    }

    // Identifier / keyword
    if (isIdentStart(c)) {
        size_t start = i;
        i++;
        while (i < src.size() && isIdentChar(src[i])) i++;
        string lex = src.substr(start, i - start);
        if (lex == "print") return Token{TokType::KW_PRINT, lex};
        return Token{TokType::IDENT, lex};
    }

    throw lexError(string("unexpected character '") + c + "'");
}
