#include "parser.h"
#include <utility>

Parser::Parser(std::string source) : lex(std::move(source)) {
    cur = lex.next();
}

void Parser::advance() {
    cur = lex.next();
}

[[noreturn]] void Parser::error(const std::string& msg) const {
    throw std::runtime_error("Parse error: " + msg + " (at token '" + cur.lexeme + "')");
}

void Parser::expect(TokType t, const std::string& what) {
    if (cur.type != t) error("expected " + what);
    advance();
}

Program Parser::parseProgram() {
    Program p;
    while (cur.type != TokType::END) {
        p.stmts.push_back(parseStmt());
    }
    return p;
}

std::unique_ptr<Stmt> Parser::parseStmt() {
    if (cur.type == TokType::KW_PRINT) {
        advance();
        auto value = parseExpr();
        expect(TokType::SEMI, "';'");
        return std::make_unique<PrintStmt>(std::move(value));
    }

    if (cur.type == TokType::IDENT) {
        std::string name = cur.lexeme;
        advance();
        expect(TokType::EQUAL, "'='");
        auto value = parseExpr();
        expect(TokType::SEMI, "';'");
        return std::make_unique<AssignStmt>(std::move(name), std::move(value));
    }

    error("expected a statement (assignment or print)");
}

std::unique_ptr<Expr> Parser::parseExpr() {
    auto left = parseTerm();
    while (cur.type == TokType::PLUS || cur.type == TokType::MINUS) {
        TokType opTok = cur.type;
        advance();
        auto right = parseTerm();
        char op = (opTok == TokType::PLUS) ? '+' : '-';
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseTerm() {
    auto left = parseUnary();
    while (cur.type == TokType::STAR || cur.type == TokType::SLASH) {
        TokType opTok = cur.type;
        advance();
        auto right = parseUnary();
        char op = (opTok == TokType::STAR) ? '*' : '/';
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseUnary() {
    if (cur.type == TokType::MINUS) {
        advance();
        auto inner = parseUnary();
        return std::make_unique<UnaryExpr>('-', std::move(inner));
    }
    return parseFactor();
}

std::unique_ptr<Expr> Parser::parseFactor() {
    if (cur.type == TokType::NUMBER) {
        long long v = cur.number;
        advance();
        return std::make_unique<NumberExpr>(v);
    }

    if (cur.type == TokType::IDENT) {
        std::string name = cur.lexeme;
        advance();
        return std::make_unique<VarExpr>(std::move(name));
    }

    if (cur.type == TokType::LPAREN) {
        advance();
        auto e = parseExpr();
        expect(TokType::RPAREN, "')'");
        return e;
    }

    error("expected NUMBER, IDENT, or '('");
}
