#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Lexer/tokens.h"  // token and enum definitions

extern Token tokens[1000];
extern int tokenCount;
extern int currentToken;

// ---- Token Loading ----
void loadTokensFromFile(const char *filename);

// ---- Utility ----
void match(int expected);
Token getCurrentToken();
int check(int expected);

// ---- Parser Entry ----
void parseProgram();

// ---- Grammar Rules ----
void parseFunctionList();
void parseFunction();
void parseStatementList();
void parseStatement();
void parseDeclarationStatement();
void parseAssignmentStatement();
void parseConditionalStatement();
void parseLoopStatement();
void parseExpression();
void parseExpressionTail();
void parseTerm();
void parseTermTail();
void parseFactor();
void parseBooleanExpression();
void parseRelOp();

#endif