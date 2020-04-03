#include "Lexical.hpp"
// for debug
inline void out(const char* str){
    std::cout << str << std::endl;
}

std::vector<ReservedWord> Dict={{"if",IF},{"else",ELSE},
      {"int",INT},{"double",DOUBLE},{"string", STRING},
      {"return",RETURN},{"void",VOID},{"while",WHILE}};

static void printToken(const std::vector<char> &t, TokenType tt){
    int s = t.size();
    switch (tt){
        case IF:std::cout<< "reserved word: if\n";break;
        case ELSE:std::cout<< "reserved word: else\n";break;
        case INT:std::cout<< "reserved word: int\n";break;
        case DOUBLE:std::cout<< "reserved word: double\n";break;
        case STRING:std::cout<< "reserved word: string\n";break;
        case RETURN:std::cout<< "reserved word: return\n";break;
        case VOID:std::cout<< "reserved word: void\n";break;
        case WHILE:std::cout<< "reserved word: while\n";break;
        case ID:std::cout<< "ID, name = ";break;
        case INT_NUM:std::cout<< "integer number: ";break;
        case CONST_STRING:std::cout<< "string constant: ";break;
        case DOUBLE_NUM:std::cout<< "double number: ";break;
        case MULTICOMMENT:std::cout<< "multi comment: ";break;
        case SINGLECOMMENT:std::cout<< "single comment: ";break;
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
        case LPAREN:std::cout<< "(\n";break;
        case RPAREN:std::cout<< ")\n";break;
        case LBRACE:std::cout<< "{\n";break;
        case RBRACE:std::cout<< "}\n";break;
        case LSQBRAC:std::cout<< "[\n";break;
        case RSQBRAC:std::cout<< "]\n";break;
        case SEMI:std::cout<< ";\n";break;
        case COMMA:std::cout<< ",\n";break;
        default:/*正常不会显示*/
            std::cout<< "invisible token\n";break;
    }
    switch (tt)
    {
    case ID:case INT_NUM:case CONST_STRING:case DOUBLE_NUM:
    case MULTICOMMENT: case SINGLECOMMENT:
        for(int i = 0; i < s; i++){
            std::cout << t[i];
        }
        std::cout << std:: endl;
        break;    
    default:
        break;
    }
}

bool cmp(const std::vector<char> &token, const char* reserved){
    int s = token.size();
    for(int i = 0; i < s; i++){
        if(token[i] != reserved[i]){
            return false;
        }
    }
    return true;
}

static void isReservedWord(TokenType &tt, const std::vector<char> &token){
    int s = Dict.size();
    int len = token.size();
    for(int i = 0; i < s; i++){
        if(strlen(Dict[i].str) == len && cmp(token, Dict[i].str)){
            tt = Dict[i].tokentype;
            return;
        }
    }
}

TokenType getToken(std::ifstream &f){
    static std::vector<char> token;
    static char nextch;
    token.clear();
    StateType state = START;
    TokenType thisToken;
    while(state != DONE){
        nextch = f.peek();
        if(nextch!=EOF){
            if(nextch == ' ' || nextch == '\n' || nextch == '\t'){
                if(state == START){
                    f.get();
                    return BLANKCHAR;
                }
            }
        }
        else if(state == START){            
            return ENDOFFILE;
        }
        switch (state){
            case START:{
                if(isdigit(nextch)){
                    state = INNUM;
                    thisToken = INT_NUM;
                    token.push_back(nextch);                    
                }
                else if(isalpha(nextch) || nextch == '_'){
                    state = INID;
                    thisToken = ID;
                    token.push_back(nextch);                    
                }
                else if(nextch == '.'){
                    state = INDOUBLE;
                    token.push_back('0');
                    token.push_back(nextch);
                }
                else{
                    switch(nextch){
                        case '\"':{
                            thisToken = CONST_STRING;
                            state = INSTRING;
                        }
                        break;

                        case '/':state = OVERORCOMMENT;token.push_back(nextch);break;

                        case '{':thisToken = LBRACE;state = DONE;break;
                        case '}':thisToken = RBRACE;state = DONE;break;
                        case '(':thisToken = LPAREN;state = DONE;break;
                        case ')':thisToken = RPAREN;state = DONE;break;
                        case '[':thisToken = LSQBRAC;state = DONE;break;
                        case ']':thisToken = RSQBRAC;state = DONE;break;
                        case ';':thisToken = SEMI;state = DONE;break;
                        case ',':thisToken = COMMA;state = DONE;break;
                        case '+':thisToken = PLUS;state = DONE;break;
                        case '-':thisToken = MINUS;state = DONE;break;
                        case '*':thisToken = TIMES;state = DONE;break;

                        case '=':state = ASSIGNOREQ;break;
                        case '<':state = LTORLEQ;break;
                        case '>':state = GTORGEQ;break;
                        case '!':state = INNEQ;break;
                        
                        default:
                            return ERROR;
                    }
                }
                f.get();
            }
            break;

            case ASSIGNOREQ:{
                if(nextch == '='){
                    f.get();
                    token.push_back(nextch);
                    thisToken = EQ;
                    state = DONE;
                }
                else{
                    thisToken = ASSIGN;
                    state = DONE;
                }
            }
            break;

            case LTORLEQ:{
                if(nextch == '='){
                    f.get();
                    token.push_back(nextch);
                    thisToken = LEQ;
                    state = DONE;
                }
                else{
                    thisToken = LT;
                    state = DONE;
                }

            }
            break;

            case GTORGEQ:{
                if(nextch == '='){
                    f.get();
                    token.push_back(nextch);
                    thisToken = GEQ;
                    state = DONE;
                }
                else{
                    thisToken = GT;
                    state = DONE;
                }
            }
            break;

            case INNEQ:{
                if(nextch == '='){
                    f.get();
                    token.push_back(nextch);
                    thisToken = NEQ;
                    state = DONE;
                }
                else{
                    return ERROR;
                }

            }
            break;

            case INID:{
                if(isalpha(nextch) || isdigit(nextch) || nextch =='_'){
                    f.get();
                    token.push_back(nextch);
                }
                else{
                    isReservedWord(thisToken, token);
                    state = DONE;
                }

            }
            break;

            case INNUM:{
                if(isdigit(nextch)){
                    f.get();
                    token.push_back(nextch);
                }
                else if(nextch == 'e'){
                    f.get();
                    token.push_back(nextch);
                    thisToken = DOUBLE_NUM;
                    state = INSCIENTIFIC;
                }
                else if(nextch == '.'){
                    f.get();
                    token.push_back(nextch);
                    thisToken = DOUBLE_NUM;
                    state = INDOUBLE;
                }
                else{
                    thisToken = INT_NUM;//重复代码，可不写
                    state = DONE;
                }
            }
            break;

            case OVERORCOMMENT:{
                if(nextch == '*'){
                    f.get();
                    token.push_back(nextch);
                    state = INMULTICOMMENT;
                    thisToken = MULTICOMMENT;
                }
                else if(nextch == '/'){
                    f.get();
                    token.push_back(nextch);
                    state = INSINGLECOMMENT;
                    thisToken = SINGLECOMMENT;
                }
                else{
                    thisToken = OVER;
                    state = DONE;
                }
            }
            break;

            case INMULTICOMMENT:{
                if(nextch == EOF){
                    thisToken = MULTICOMMENT;
                    state = DONE;
                }
                else{
                    f.get();
                    token.push_back(nextch);
                    if(nextch == '*'){
                        thisToken = MULTICOMMENT;
                        state = ENDMULTICOMMENT;
                    }
                }
            }
            break;

            case ENDMULTICOMMENT:{
                if(nextch == EOF){
                    thisToken = MULTICOMMENT;
                    state = DONE;
                }
                else{
                    f.get();
                    token.push_back(nextch);
                    if(nextch == '/'){
                        state = DONE;
                    }
                    else{
                        state = INMULTICOMMENT;
                    }
                }
            }
            break;

            case INSINGLECOMMENT:{
                if(nextch == EOF || nextch == '\n'){
                    thisToken = SINGLECOMMENT;
                    state = DONE;
                }
                else{
                    f.get();
                    token.push_back(nextch);
                }
            }
            break;

            case INSTRING:{
                f.get();
                if(nextch == '\"'){
                    thisToken = CONST_STRING;
                    state = DONE;
                }
                else{
                    token.push_back(nextch);
                }
            }
            break;

            case TRANSEFERMEANING:{

            }

            case INDOUBLE:{
                if(isdigit(nextch)){
                    f.get();
                    token.push_back(nextch);
                }
                else if(nextch == 'e'){
                    f.get();
                    token.push_back(nextch);
                    thisToken = DOUBLE_NUM;
                    state = INSCIENTIFIC;
                }
                else{
                    thisToken = DOUBLE_NUM;
                    state = DONE;
                }
            }
            break;

            case INSCIENTIFIC:{
                if(isdigit(nextch)){
                    f.get();
                    token.push_back(nextch);
                }
                else{
                    thisToken = DOUBLE_NUM;
                    state = DONE;
                }
            }
            break;
        }
    }

    if(PRINTTOKEN){
        printToken(token, thisToken);
    }
    return thisToken;
}