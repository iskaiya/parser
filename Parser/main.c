#include "parser.h"

int main() {
    loadTokensFromFile("../Lexer/Symbol Table.txt");
    parseProgram();
    return 0;
}