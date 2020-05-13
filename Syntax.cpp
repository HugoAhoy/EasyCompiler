#include "Syntax.hpp"
#include "Lexical.hpp"

void syntaxError(std::string err){
    std::cout << err << std::endl;
}

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
    TreeNode * t = new TreeNode();
    if (t ==nullptr){
        std::cout << "out of memory limit" << std::endl;
    }
    else{
        t->sibling = nullptr;
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
        syntaxError("unexpected token in match");
    }
}

TreeNode *stmt_sequence(std::ifstream &fin, Token &token){
    TreeNode *t, *root;
    t = root = block(fin, token);
    while(token.type != ERROR && token.type != ENDOFFILE && token.type != RBRACE){
        t->sibling = block(fin, token);
        while(t->sibling != nullptr){
            t = t->sibling;
        }
    }
    if(token.type == ERROR){
        std::cout << "illegal token" << std::endl;
    }
    return root;
}

TreeNode *block(std::ifstream &fin, Token &token){
    TreeNode *t; 
    if(token.type == LBRACE){
        match(fin, token, LBRACE);
        t = stmt_sequence(fin, token);
        match(fin, token, RBRACE);
    }
    else{
        t = statement(fin, token);
    }
    return t;
}

TreeNode * statement(std::ifstream &fin, Token &token){
    TreeNode * t = nullptr;
    switch (token.type) {
        case IF : t = if_stmt(fin, token); break;
        case ID : t = assign_stmt(fin, token); match(fin, token, SEMI); break;
        case FOR : t = for_stmt(fin, token);break;
        case WHILE : t = while_stmt(fin, token);break;
        case INT : case DOUBLE : case STRING :
        t = declare_stmt(fin, token); match(fin, token, SEMI); break;
        default : syntaxError("unexpected token in statement");
                break;
    } /* end case */
    return t;
}

TreeNode * if_stmt(std::ifstream &fin, Token &token){
    TreeNode * t = newStmt(IfStmt);
    if(t == nullptr){
        return t;
    }
    match(fin, token, IF);
    match(fin, token, LPAREN);
    t->offspring.push_back(exp(fin, token));
    match(fin, token, RPAREN);
    t->offspring.push_back(block(fin, token));
    // std::cout << (token.type == ELSE) << std::endl;
    if (token.type == ELSE) {
        // std::cout << "true" << std::endl;
        match(fin, token, ELSE);
        t->offspring.push_back(block(fin, token));
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
    t->offspring.push_back(block(fin, token));
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
    match(fin, token, SEMI);
    t->offspring.push_back(exp(fin, token));
    match(fin, token, SEMI);
    t->offspring.push_back(assign_stmt(fin, token));
    match(fin, token, RPAREN);
    t->offspring.push_back(block(fin, token));
    return t;
}

TreeNode *declare_stmt(std::ifstream &fin, Token &token){
    TreeNode * t= newStmt(DeclareStmt);
    if(t == nullptr){
        return t;
    }
    t->DeclareType = token.type;
    match(fin, token, token.type);
    t->name = token.attr;
    match(fin, token, ID);
    if(token.type == ASSIGN){
        match(fin, token, ASSIGN);
        t->offspring.push_back(exp(fin, token));
    }
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
    TreeNode * t = nullptr;
    std::cout << token.attr << std:: endl;
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
                t->name = token.attr;
            }
            match(fin, token, ID);
            break;
        case LPAREN :
            match(fin, token, LPAREN);
            t = exp(fin, token);
            match(fin, token, RPAREN);
            break;
        default:
            syntaxError("unexpected token in factor");
            break;
        }
    return t;
}

TreeNode *parse(std::ifstream &fin){
    Token token = getTokenWithoutBlank(fin);
    return stmt_sequence(fin, token);
}

void Traverse(TreeNode *root, int level){
    for(int i = 0; i < (level-1) * 4; i++){
        std::cout << " " ;
    }

    if(level > 0){
        std::cout << " |--";
    }

    if(root->isStmt){
        switch(root->kind.stmt){
            case IfStmt: std::cout << "if-stmt" << std::endl;break;
            case ForStmt: std::cout << "for-stmt" << std::endl;break;
            case WhileStmt: std::cout << "while-stmt" << std::endl;break;
            case AssignStmt: std::cout << "assign-stmt " << "ID: " << root->name << std::endl;break;
            case DeclareStmt: std::cout << "declare-stmt " << "ID: " << root->name << std::endl;break;
        }
    }
    else{
        switch(root->kind.exp){
            case IdExp:std::cout << "ID:" << root->name << std::endl;break;
            case OpExp:
                std::cout << "Op";
                switch(root->op){
                    case ASSIGN:std::cout<< "=\n";break;
                    case EQ:std::cout<< "==\n";break;
                    case LT:std::cout<< "<\n";break;
                    case GT:std::cout<< ">\n";break;
                    case LEQ:std::cout<< "<=\n";break;
                    case GEQ:std::cout<< ">=\n";break;
                    case NEQ:std::cout<< "!=\n";break;
                    case PLUS:std::cout<< "+\n";break;
                    case MINUS:std::cout<< "-\n";break;
                    case TIMES:std::cout<< "*\n";break;
                    case OVER:std::cout<< "/\n";break;
                }
                break;
            case ConstExp:std::cout << "Const:" << root->name << std::endl;break;
        }
    }

    int s = root->offspring.size();
    for(int i = 0; i < s -1; i++){
        // for(int i = 0; i < level * 4; i++){
        //     std::cout << " " ;
        // }
        // std::cout << " {" << std::endl;

        Traverse(root->offspring[i], level+1);
        std::cout << std::endl;
        // for(int i = 0; i < level * 4; i++){
        //     std::cout << " " ;
        // }
        // std::cout << " }" << std::endl;
    }
    if(s > 0){
        Traverse(root->offspring.back(), level+1);
    }

    if(root->sibling != nullptr){
        Traverse(root->sibling, level);
    }
    return;
}