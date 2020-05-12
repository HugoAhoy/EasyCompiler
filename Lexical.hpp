#include "General.hpp"
#ifndef _LEXICAL_HPP_
#define _LEXICAL_HPP_


TokenType getToken(std::ifstream &f);
Token getTokenWithInfo(std::ifstream &fin);
Token getTokenWithoutBlank(std::ifstream &fin);
void printToken(const std::vector<char> &t, TokenType tt)
#endif