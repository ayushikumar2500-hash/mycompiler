#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#include <iostream>
#include <string>

// -------------------- Pretty-print AST --------------------

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

static void printProgramAST(const Program& prog) {
    for (const auto& st : prog.stmts) {
        printStmt(st.get());
    }
}

// -------------------- Lexer test --------------------

static void testLexer(const std::string& program) {
    std::cout << "====================\n";
    std::cout << "1) LEXER OUTPUT\n";
    std::cout << "====================\n";

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

// -------------------- Parser test --------------------

static Program testParser(const std::string& program) {
    std::cout << "====================\n";
    std::cout << "2) PARSER OUTPUT (AST pretty-print)\n";
    std::cout << "====================\n";

    Parser parser(program);
    Program ast = parser.parseProgram();

    printProgramAST(ast);
    std::cout << "\n";

    return ast; // returned by value; Program holds unique_ptrs safely
}

// -------------------- Interpreter test --------------------

static void testInterpreter(const Program& ast) {
    std::cout << "====================\n";
    std::cout << "3) INTERPRETER OUTPUT (program run)\n";
    std::cout << "====================\n";

    Interpreter interp;
    interp.run(ast);

    std::cout << "\n";
}

// -------------------- Main --------------------

int main() {
    // This program exercises:
    // - assignment
    // - variable references
    // - precedence (* before +)
    // - parentheses
    // - print
    std::string program =
        "x = 2 + 3;\n"
        "y = x * 4 + 1;\n"
        "print y;\n"
        "print (2 + 3) * 4;\n"
        "z = (1 + 2) * (3 + 4);\n"
        "print z;\n";

    std::cout << "SOURCE PROGRAM:\n";
    std::cout << "--------------------\n";
    std::cout << program;
    std::cout << "--------------------\n\n";

    try {
        testLexer(program);
        Program ast = testParser(program);
        testInterpreter(ast);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
