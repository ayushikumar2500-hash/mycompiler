#include "lexer.h"
#include <iostream>

int main() {
    std::string program =
        "x = 2 + 3;\n"
        "print x;\n";

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

    return 0;
}
