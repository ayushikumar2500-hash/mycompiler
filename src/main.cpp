#include "lexer.h"
#include "parser.h"

#include <iostream>

static void printExpr(const Expr* e) {
    if (auto n = dynamic_cast<const NumberExpr*>(e)) {
        std::cout << n->value;
        return;
    }
    if (auto v = dynamic_cast<const VarExpr*>(e)) {
        std::cout << v->name;
        return;
    }
    if (auto b = dynamic_cast<const BinaryExpr*>(e)) {
        std::cout << "(";
        printExpr(b->left.get());
        std::cout << " " << b->op << " ";
        printExpr(b->right.get());
        std::cout << ")";
        return;
    }
    std::cout << "<unknown-expr>";
}

static void printStmt(const Stmt* s) {
    if (auto a = dynamic_cast<const AssignStmt*>(s)) {
        std::cout << a->name << " = ";
        printExpr(a->value.get());
        std::cout << ";\n";
        return;
    }
    if (auto p = dynamic_cast<const PrintStmt*>(s)) {
        std::cout << "print ";
        printExpr(p->value.get());
        std::cout << ";\n";
        return;
    }
    std::cout << "<unknown-stmt>\n";
}

int main() {
    std::string program =
        "x = 2 + 3;\n"
        "y = x * 4 + 1;\n"
        "print y;\n"
        "print (2 + 3) * 4;\n";


    Lexer lex(program);

    while (true) {
        Token t = lex.next();
        std::cout << tokTypeToString(t.type);

        if (t.type == TokType::IDENT) {
            std::cout << "(" << t.lexeme << ")";
        } else if (t.type == TokType::NUMBER) {
            std::cout << "(" << t.number << ")";
        }

        std::cout << "\n";

        if (t.type == TokType::END) break;
    }

    try {
        Parser parser(program);
        Program ast = parser.parseProgram();

        std::cout << "Parsed program (AST pretty print):\n";
        for (const auto& st : ast.stmts) {
            printStmt(st.get());
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
