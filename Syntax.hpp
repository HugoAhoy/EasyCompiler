#include "General.hpp"
#ifndef _SYNTAX_HPP_
#define _SYNTAX_HPP_

TreeNode * stmt_sequence(std::ifstream &fin, Token &token);
TreeNode * statement(std::ifstream &fin, Token &token);
TreeNode * if_stmt(std::ifstream &fin, Token &token);
TreeNode * repeat_stmt(std::ifstream &fin, Token &token);
TreeNode * assign_stmt(std::ifstream &fin, Token &token);
TreeNode * for_stmt(std::ifstream &fin, Token &token);
TreeNode * while_stmt(std::ifstream &fin, Token &token);
TreeNode * exp(std::ifstream &fin, Token &token);
TreeNode * simple_exp(std::ifstream &fin, Token &token);
TreeNode * term(std::ifstream &fin, Token &token);
TreeNode * factor(std::ifstream &fin, Token &token);
TreeNode * declare_stmt(std::ifstream &fin, Token &token);
TreeNode *block(std::ifstream &fin, Token &token)
void match(std::ifstream &fin, Token &token);
void syntaxError(const char * str);

#endif // !_SYNTAX_HPP_