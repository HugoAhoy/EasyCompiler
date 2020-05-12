// 定义全局使用的一些枚举变量和结构体
#ifndef _GENERAL_HPP_
#define _GENERAL_HPP_
#include "fstream"
#include "iostream"
#include "string"
#include "cctype"
#include "vector"
#include "cstring"

extern bool PRINTTOKEN;

typedef enum{
    // reserved words
    IF, ELSE, INT, DOUBLE,STRING,RETURN,VOID,WHILE, FOR,
    // constant and variable
    ID, INT_NUM,CONST_STRING, DOUBLE_NUM,
    // operators
    ASSIGN,EQ,LT,GT,LEQ,GEQ,NEQ,PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,LBRACE,RBRACE,LSQBRAC,RSQBRAC,SEMI,COMMA,
    // other
    ENDOFFILE, BLANKCHAR,ERROR, MULTICOMMENT,SINGLECOMMENT
}TokenType;

/* states in scanner DFA */
typedef enum{
    START,INMULTICOMMENT,ENDMULTICOMMENT,INNUM,INID,INSTRING,INDOUBLE,OVERORCOMMENT,
    ASSIGNOREQ,LTORLEQ,GTORGEQ,INNEQ,TRANSEFERMEANING,INSCIENTIFIC,INSINGLECOMMENT,DONE}
   StateType;

typedef struct
    { char const *str;
      TokenType tokentype;
    } ReservedWord;

typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfStmt,ForStmt,WhileStmt,AssignStmt,DeclareStmt} StmtKind;
typedef enum {OpExp,ConstExp,IdExp} ExpKind;

typedef struct Token{
    TokenType type;
    std::string attr;
    Token(TokenType _type, std::string _attr):type(_type), attr(_attr){}
}Token;

typedef struct TreeNode{
    bool isStmt;
    TreeNode *sibling;
    std::vector<TreeNode*> offspring; 
    union { StmtKind stmt; ExpKind exp;} kind;
    TokenType op;
    int val;
    std::string name;
    TokenType DeclareType;
    //  ExpType type; /* for type checking of exps */
}TreeNode;

#endif