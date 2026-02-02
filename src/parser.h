#pragma once
#include "ast.h"
#include "lexer.h"
#include <stdexcept>
#include <string>

class Parser {
public:
    explicit Parser(std::string source);

    Program parseProgram();

private:
    Lexer lex;
    Token cur;

    void advance();
    [[noreturn]] void error(const std::string& msg) const;
    void expect(TokType t, const std::string& what);

    // Grammar:
    // program  := stmt* END
    // stmt     := IDENT '=' expr ';' | 'print' expr ';'
    // expr     := term ( '+' term )*
    // term     := factor ( '*' factor )*
    // factor   := NUMBER | IDENT | '(' expr ')'

    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();
};
