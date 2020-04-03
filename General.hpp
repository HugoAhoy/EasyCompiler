// 定义全局使用的一些枚举变量和结构体
#ifndef _GENERAL_HPP_
#define _GENERAL_HPP_
#include "fstream"
#include "iostream"
#include "string"
#include "cctype"
#include "vector"
#include "cstring"

extern bool PRINTTOKEN = false;

// const int MAXTOKENLEN = 40;

// char tokenString[MAXTOKENLEN+1];

typedef enum{
    // keywords
    IF, ELSE, INT, DOUBLE,STRING,RETURN,VOID,WHILE,
    // const and variable
    ID, INT_NUM,CONST_STRING, DOUBLE_NUM,
    // operators
    ASSIGN,EQ,LT,GT,LEQ,GEQ,NEQ,PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,LBRACE,RBRACE,LSQBRAC,RSQBRAC,SEMI,COMMA,
    // other
    ENDOFFILE, BLANKCHAR,ERROR, COMMENT
}TokenType;

/* states in scanner DFA */
typedef enum{
    START,INCOMMENT,INNUM,INID,INSTRING,INDOUBLE,OVERORCOMMENT,ASSIGNOREQ,LTORLEQ,GTORGEQ,INNEQ,DONE}
   StateType;

typedef struct
    { char* str;
      TokenType tokentype;
    } ReservedWord;

#endif