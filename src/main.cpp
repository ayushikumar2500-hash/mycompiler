#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#include <iostream>
#include <string>

// AST pretty printer (just for sanity)
static void printExpr(const Expr* e) {
    if (auto n = dynamic_cast<const NumberExpr*>(e)) {
        std::cout << n->value;
        return;
    }
    if (auto v = dynamic_cast<const VarExpr*>(e)) {
        std::cout << v->name;
        return;
    }
    if (auto u = dynamic_cast<const UnaryExpr*>(e)) {
        std::cout << "(" << u->op;
        printExpr(u->expr.get());
        std::cout << ")";
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
    std::cout << "<expr?>";
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
    std::cout << "<stmt?>\n";
}

static void printProgramAST(const Program& prog) {
    for (const auto& st : prog.stmts) printStmt(st.get());
}

static void testLexer(const std::string& program) {
    std::cout << "=== TOKENS ===\n";
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
    std::cout << "\n";
}

int main() {
    std::string program =
        "print 10 - 3;\n"          // 7
        "print 10 - 3 - 2;\n"      // (10-3)-2 = 5 (left-associative)
        "print 20 / 5;\n"          // 4
        "print 7 / 2;\n"           // 3 (integer division)
        "print 2 + 3 * 4;\n"       // 14
        "print (2 + 3) * 4;\n"     // 20
        "print 8 / 2 * 3;\n"       // (8/2)*3 = 12
        "print 8 / (2 * 3);\n"     // 8/6 = 1
        "x = 5;\n"
        "print x - 12;\n"          // -7
        "print -x;\n"              // -5
        "print -(2 + 3) * 4;\n"    // -20
        "print -(-7);\n";          // 7

    std::cout << "SOURCE:\n" << program << "\n";

    try {
        testLexer(program);

        Parser parser(program);
        Program ast = parser.parseProgram();

        std::cout << "=== AST (pretty) ===\n";
        printProgramAST(ast);
        std::cout << "\n";

        std::cout << "=== RUN ===\n";
        Interpreter interp;
        interp.run(ast);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
