#include "General.hpp"
#include "Lexical.hpp"
#include "Syntax.hpp"

bool PRINTTOKEN = true;

void OpenSourceFile(char const *filename, std::ifstream &f){
    f.open(filename, std::ios::in);
}

int main(){
    std::ifstream fin;
    char const *filename = "test.cm";
    OpenSourceFile(filename, fin);
    // while(getToken(fin) != ENDOFFILE);
    TreeNode *root = parse(fin);
    std::cout << std::endl;
    std::cout << std::endl;
 
    // print syntax tree
    Traverse(root, 0);

    // 关闭源文件
    fin.close();
}