#pragma once
#include "ast.h"
#include <string>
#include <unordered_map>

class Interpreter {
public:
    void run(const Program& program);

private:
    std::unordered_map<std::string, long long> env;

    void execStmt(const Stmt* s);
    long long evalExpr(const Expr* e);
};
