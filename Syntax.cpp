#include "Syntax.hpp"
#include "Lexical.hpp"

TreeNode * newStmt(StmtKind stmtKind){
    TreeNode * t = new TreeNode();
    if (t==nullptr){
        std::cout << "out of memory limit" << std::endl;
    }
    else{
        t->sibling = nullptr;
        t->isStmt = true;
        t->kind.stmt = stmtKind;
    }
    return t;
}

TreeNode * newExp(ExpKind expKind){
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    if (t==nullptr){
        std::cout << "out of memory limit" << std::endl;
    }
    else {
        t->sibling = NULL;
        t->isStmt = false;
        t->kind.exp = expKind;
    }
    return t;
}

void match(std::ifstream &fin, Token &token, TokenType expected){
    if (token.type == expected){
        token = getTokenWithoutBlank(fin);
    }
    else{
        syntaxError("unexpected token -> ");
    }
}

TreeNode *stmt_sequence(std::ifstream &fin, Token &token){
    TreeNode *t, *root;
    t = root = block(fin, token);
    while(token.type != ERROR || token.type != ENDOFFILE || token.type != RBRACE){
        t->sibling = statement(fin, token);
        t = t->sibling;
    }
    return root;
}

TreeNode *block(std::ifstream &fin, Token &token){
    TreeNode *t; 
    if(token.type == LBRACE){
        match(fin, token, LBRACE);
        t = stmt_sequence(fin, token);
    }
    else{
        t = statement(fin, token);
        match(fin, token, SEMI);
    }
}

TreeNode * statement(std::ifstream &fin, Token &token){
    TreeNode * t = nullptr;
    switch (token.type) {
        case IF : t = if_stmt(fin, token); break;
        case ID : t = assign_stmt(fin, token); break;
        case FOR : t = for_stmt(fin, token);break;
        case WHILE : t = while_stmt(fin, token);break;
        case INT : case DOUBLE : case STRING :
        t = declare_stmt(fin, token); break;
        default : syntaxError("unexpected token");
                break;
    } /* end case */
    return t;
}

TreeNode * if_stmt(std::ifstream fin, Token &token){
    TreeNode * t = newStmt(IfStmt);
    if(t == nullptr){
        return t;
    }
    match(fin, token, IF);
    match(fin, token, LPAREN);
    t->offspring.push_back(exp(fin, token));
    match(fin, token, RPAREN);
    match(fin, token, LBRACE);
    t->offspring.push_back(stmt_sequence(fin, token));
    match(fin, token, RBRACE);
    if (token.type == ELSE) {
        match(fin, token, ELSE);
        match(fin, token, LBRACE);
        t->offspring.push_back(stmt_sequence(fin, token));
        match(fin, token, RBRACE);
    }
    return t;
}

TreeNode * assign_stmt(std::ifstream &fin, Token &token){
    TreeNode * t = newStmt(AssignStmt);
    if(t == nullptr){
        return t;
    }
    if (token.type == ID){
        t->name = token.attr;
    }
    match(fin, token, ID);
    match(fin, token, ASSIGN);
    t->offspring.push_back(exp(fin, token));
    match(fin, token, SEMI);
    return t;
}

TreeNode * while_stmt(std::ifstream &fin, Token &token){
    TreeNode *t = newStmt(WhileStmt);
    if(t == nullptr){
        return t;
    }
    match(fin, token, WHILE);
    match(fin, token, LPAREN);
    t->offspring.push_back(exp(fin, token));
    match(fin, token, RPAREN);
    match(fin, token, LBRACE);
    t->offspring.push_back(stmt_sequence(fin, token));
    match(fin, token, RBRACE);
    return t;
}

TreeNode *for_stmt(std::ifstream &fin, Token &token){
    TreeNode *t = newStmt(ForStmt);
    if(t == nullptr){
        return t;
    }
    match(fin, token, FOR);
    match(fin, token, LPAREN);
    t->offspring.push_back(declare_stmt(fin, token));
    t->offspring.push_back(exp(fin, token));
    t->offspring.push_back(exp(fin, token));
    match(fin, token, RPAREN);
    match(fin, token, LBRACE);
    t->offspring.push_back(stmt_sequence(fin, token));
    match(fin, token, RBRACE);
    return t;
}

TreeNode *declare_stmt(std::ifstream &fin, Token &token){
    TreeNode * t= newStmt(DeclareStmt);
    if(t == nullptr){
        return t;
    }
    match(fin, token, token.type);
    t->DeclareType = token.type;
    t->name = token.attr;
    match(fin, token, ID);
    match(fin, token, SEMI);
    return t;
}

TreeNode * exp(std::ifstream &fin, Token &token){
    TreeNode * t = simple_exp(fin, token);
    if (token.type==LT||token.type==EQ || token.type == GT|| token.type == LEQ|| token.type == GEQ|| token.type == NEQ) {
        TreeNode * p = newExp(OpExp);
        if (p!=nullptr){
            p->offspring.push_back(t);
            p->op = token.type;
            t = p;
        }
        match(fin, token, token.type);
        if (t!= nullptr){
            t->offspring.push_back(simple_exp(fin, token));
        }
    }
    return t;
}

TreeNode * simple_exp(std::ifstream &fin, Token &token){
    TreeNode * t = term(fin, token);
    while (token.type==PLUS||token.type==MINUS){
        TreeNode * p = newExp(OpExp);
        if (p!=nullptr){
            p->offspring.push_back(t);
            p->op = token.type;
            t = p;
            match(fin, token, token.type);
            t->offspring.push_back(term(fin, token));
        }
    }
    return t;
}

TreeNode * term(std::ifstream &fin, Token &token){
    TreeNode * t = factor(fin, token);
    while (token.type==TIMES||token.type==OVER){
        TreeNode * p = newExp(OpExp);
        if (p!=nullptr){
            p->offspring.push_back(t);
            p->op = token.type;
            t = p;
            match(fin, token, token.type);
            t->offspring.push_back(factor(fin, token));
        }
    }
    return t;
}

TreeNode * factor(std::ifstream &fin, Token &token){
    TreeNode * t = NULL;
    switch (token.type) {
        case INT_NUM : case DOUBLE_NUM : case CONST_STRING:
            t = newExp(ConstExp);
            if (t!=nullptr){
                t->name = token.attr;
            }
            match(fin, token, token.type);
            break;
        case ID :
            t = newExp(IdExp);
            if (t != nullptr){
                t->name = token.type;
            }
            match(fin, token, ID);
            break;
        case LPAREN :
            match(fin, token, LPAREN);
            t = exp(fin, token);
            match(fin, token, RPAREN);
            break;
        default:
            syntaxError("unexpected token");
            break;
        }
    return t;
}

TreeNode *parse(std::ifstream &fin){
    Token token = getTokenWithoutBlank(fin);
    return stmt_sequence(fin, token);
}