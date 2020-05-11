#include "General.hpp"
#ifndef _SYNTAX_HPP_
#define _SYNTAX_HPP_

TreeNode * stmt_sequence(std::ifstream &fin);
TreeNode * statement(std::ifstream &fin);
TreeNode * if_stmt(std::ifstream &fin);
TreeNode * repeat_stmt(std::ifstream &fin);
TreeNode * assign_stmt(std::ifstream &fin);
TreeNode * for_stmt(std::ifstream &fin);
TreeNode * while_stmt(std::ifstream &fin);
TreeNode * exp(std::ifstream &fin);
TreeNode * simple_exp(std::ifstream &fin);
TreeNode * term(std::ifstream &fin);
TreeNode * factor(std::ifstream &fin);
void match(TokenType token);
void syntaxError(const char * str);

#endif // !_SYNTAX_HPP_