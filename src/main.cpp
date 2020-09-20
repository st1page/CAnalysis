#include <iostream>

#include "CLexer.h"
#include "CParser.h"
#include "CVisitor.h"

using namespace antlr4;
using namespace std;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "[error] Input sourcefile required: "<< argv[0]<<" <sourcefile>\n";
        return 1;
    }
    ifstream sourceFile;
    sourceFile.open(argv[1]);

    ANTLRInputStream input(sourceFile);
    CLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    CParser parser(&tokens);

    return 0;
}

