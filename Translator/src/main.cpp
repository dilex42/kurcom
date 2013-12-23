#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Lexer.h"
#include "Node.h"

using namespace std;
using namespace token;
using namespace node;

int main(int argc, char** argv) {

    Lexer lexer(
            "VAR{\n"
            "i:int;\n"
            "k:int;\n"
            "}\n"
            "GO{\n"
            "i = 1;\n"
            "if(i!= true){\n"
            "k = 13;"
            "}");
    Token * tok = NULL;
    std::vector<Token> result;
    int i = 0;
    do {
        tok = lexer.scan();
        if (tok != NULL) {
            result.push_back(*tok);
            cout << "Token:" << getKindString(tok->getKind()) << " with position:"
                    << tok->getLocation().toString() << endl;        
        }
        i++;
    } while ((tok == NULL) || tok->getKind() != END);

    return 0;
}
