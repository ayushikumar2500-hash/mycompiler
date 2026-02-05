#include "interpreter.h"
#include <iostream>
#include <stdexcept>

void Interpreter::run(const Program& program) {
    for (const auto& st : program.stmts) {
        execStmt(st.get());
    }
}

void Interpreter::execStmt(const Stmt* s) {
    if (auto a = dynamic_cast<const AssignStmt*>(s)) {
        long long val = evalExpr(a->value.get());
        env[a->name] = val;
        return;
    }

    if (auto p = dynamic_cast<const PrintStmt*>(s)) {
        long long val = evalExpr(p->value.get());
        std::cout << val << "\n";
        return;
    }

    throw std::runtime_error("Runtime error: unknown statement type");
}

long long Interpreter::evalExpr(const Expr* e) {
    if (auto n = dynamic_cast<const NumberExpr*>(e)) {
        return n->value;
    }

    if (auto v = dynamic_cast<const VarExpr*>(e)) {
        auto it = env.find(v->name);
        if (it == env.end()) {
            throw std::runtime_error("Runtime error: undefined variable '" + v->name + "'");
        }
        return it->second;
    }

    if (auto b = dynamic_cast<const BinaryExpr*>(e)) {
        long long L = evalExpr(b->left.get());
        long long R = evalExpr(b->right.get());

        if (b->op == '+') return L + R;
        if (b->op == '*') return L * R;

        throw std::runtime_error("Runtime error: unknown binary operator");
    }

    throw std::runtime_error("Runtime error: unknown expression type");
}
