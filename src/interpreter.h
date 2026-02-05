#pragma once
#include "ast.h"
#include <unordered_map>
#include <string>

class Interpreter {
public:
    // Run the whole program (exec each stmt)
    void run(const Program& program);

private:
    std::unordered_map<std::string, long long> env;

    // Execute statements
    void execStmt(const Stmt* s);

    // Evaluate expressions -> returns a number
    long long evalExpr(const Expr* e);
};
