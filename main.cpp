#include "General.hpp"
#include "Lexical.hpp"

bool PRINTTOKEN = true;

void OpenSourceFile(char const *filename, std::ifstream &f){
    f.open(filename, std::ios::in);
}

int main(){
    std::ifstream fin;
    char const *filename = "test.cm";
    OpenSourceFile(filename, fin);
    while(getToken(fin) != ENDOFFILE);

    // 关闭源文件
    fin.close();
}