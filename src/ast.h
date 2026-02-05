#pragma once
#include <memory>
#include <string>
#include <vector>

// ---------- Expressions ----------
struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    long long value;
    explicit NumberExpr(long long v) : value(v) {}
};

struct VarExpr : Expr {
    std::string name;
    explicit VarExpr(std::string n) : name(std::move(n)) {}
};

struct UnaryExpr : Expr {
    char op; // only '-' for now
    std::unique_ptr<Expr> expr;

    UnaryExpr(char o, std::unique_ptr<Expr> e)
        : op(o), expr(std::move(e)) {}
};

struct BinaryExpr : Expr {
    char op; // '+', '-', '*', '/'
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;

    BinaryExpr(char o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

// ---------- Statements ----------
struct Stmt {
    virtual ~Stmt() = default;
};

struct AssignStmt : Stmt {
    std::string name;
    std::unique_ptr<Expr> value;

    AssignStmt(std::string n, std::unique_ptr<Expr> v)
        : name(std::move(n)), value(std::move(v)) {}
};

struct PrintStmt : Stmt {
    std::unique_ptr<Expr> value;
    explicit PrintStmt(std::unique_ptr<Expr> v) : value(std::move(v)) {}
};

// ---------- Program ----------
struct Program {
    std::vector<std::unique_ptr<Stmt>> stmts;
};
