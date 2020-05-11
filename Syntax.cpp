#include "Syntax.hpp"
#include "Lexical.hpp"

TreeNode * statement(std::ifstream &fin){
    TreeNode * t = nullptr;
    TokenType token = getToken(fin);
    switch (token) {
        case IF : t = if_stmt(fin); break;
        case ID : t = assign_stmt(fin); break;
        case FOR : t = for_stmt(fin);break;
        case WHILE : t = while_stmt(fin);break;
        default : syntaxError("unexpected token");
                break;
    } /* end case */
    return t;
}

