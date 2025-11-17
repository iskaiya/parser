#include "parser.h"
#include "../Lexer/wordhash.h"

Token tokens[1000];
int tokenCount = 0;
int currentToken = 0;

// error message
void syntaxError(const char* message, int lineNumber, const char* lexeme) {
    if (lineNumber > 0 && lexeme && lexeme[0] != '\0')
        printf("Syntax Error at line %d: %s near '%s'\n", lineNumber, message, lexeme);
    else if (lineNumber > 0)
        printf("Syntax Error at line %d: %s\n", lineNumber, message);
    else
        printf("Syntax Error: %s near '%s'\n", message, lexeme ? lexeme : "");
}




// Token Loading

void loadTokensFromFile(const char *filename) {
    initialize_table();

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open %s\n", filename);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // skip header lines or empty lines
        if (strstr(line, "Lexeme") || strstr(line, "Token Name") || strlen(line) < 3)
            continue;

        char lexeme[100], tokenName[100];
        int lineNum;
        if (sscanf(line, "%s | %s | %d", lexeme, tokenName, &lineNum) == 3) {

            // skip comments
            if (strcmp(tokenName, "C_SINGLE_LINE") == 0 || strcmp(tokenName, "C_MULTI_LINE") == 0)
                continue;

            tokens[tokenCount].lexeme = strdup(lexeme);
            tokens[tokenCount].lineNumber = lineNum;

            int category, value;

            // look up the lexeme in the hash table
            if (hashLookup(lexeme, &category, &value)) {
                tokens[tokenCount].tokenValue = (TokenCategory)value;  // use enum directly
                tokens[tokenCount].category = category;
            } else {
    if (strcmp(tokenName, "L_IDENTIFIER") == 0)
        tokens[tokenCount].tokenValue = L_IDENTIFIER;
    else if (strcmp(tokenName, "L_BILANG_LITERAL") == 0)
        tokens[tokenCount].tokenValue = L_BILANG_LITERAL;
    else if (strcmp(tokenName, "L_LUTANG_LITERAL") == 0)
        tokens[tokenCount].tokenValue = L_LUTANG_LITERAL;
    else if (strcmp(tokenName, "L_KWERDAS_LITERAL") == 0)
        tokens[tokenCount].tokenValue = L_KWERDAS_LITERAL;
    else if (strcmp(tokenName, "L_BULYAN_LITERAL") == 0)
        tokens[tokenCount].tokenValue = L_BULYAN_LITERAL;

    // delimiters
    else if (strcmp(tokenName, "D_LPAREN") == 0)
        tokens[tokenCount].tokenValue = D_LPAREN;
    else if (strcmp(tokenName, "D_RPAREN") == 0)
        tokens[tokenCount].tokenValue = D_RPAREN;
    else if (strcmp(tokenName, "D_LBRACE") == 0)
        tokens[tokenCount].tokenValue = D_LBRACE;
    else if (strcmp(tokenName, "D_RBRACE") == 0)
        tokens[tokenCount].tokenValue = D_RBRACE;
    else if (strcmp(tokenName, "D_LBRACKET") == 0)
        tokens[tokenCount].tokenValue = D_LBRACKET;
    else if (strcmp(tokenName, "D_RBRACKET") == 0)
        tokens[tokenCount].tokenValue = D_RBRACKET;
    else if (strcmp(tokenName, "D_COMMA") == 0)
        tokens[tokenCount].tokenValue = D_COMMA;
    else if (strcmp(tokenName, "D_SEMICOLON") == 0)
        tokens[tokenCount].tokenValue = D_SEMICOLON;
    else if (strcmp(tokenName, "D_COLON") == 0)
        tokens[tokenCount].tokenValue = D_COLON;
    else if (strcmp(tokenName, "D_DOT") == 0)
        tokens[tokenCount].tokenValue = D_DOT;
    else if (strcmp(tokenName, "D_QUOTE") == 0)
        tokens[tokenCount].tokenValue = D_QUOTE;
    else if (strcmp(tokenName, "D_SQUOTE") == 0)
        tokens[tokenCount].tokenValue = D_SQUOTE;

    // operators
    else if (strcmp(tokenName, "O_PLUS") == 0)
        tokens[tokenCount].tokenValue = O_PLUS;
    else if (strcmp(tokenName, "O_MINUS") == 0)
        tokens[tokenCount].tokenValue = O_MINUS;
    else if (strcmp(tokenName, "O_MULTIPLY") == 0)
        tokens[tokenCount].tokenValue = O_MULTIPLY;
    else if (strcmp(tokenName, "O_DIVIDE") == 0)
        tokens[tokenCount].tokenValue = O_DIVIDE;
    else if (strcmp(tokenName, "O_ASSIGN") == 0)
        tokens[tokenCount].tokenValue = O_ASSIGN;
    else if (strcmp(tokenName, "O_EQUAL") == 0)
        tokens[tokenCount].tokenValue = O_EQUAL;
    else if (strcmp(tokenName, "O_NOT_EQUAL") == 0)
        tokens[tokenCount].tokenValue = O_NOT_EQUAL;
    else if (strcmp(tokenName, "O_LESS") == 0)
        tokens[tokenCount].tokenValue = O_LESS;
    else if (strcmp(tokenName, "O_GREATER") == 0)
        tokens[tokenCount].tokenValue = O_GREATER;
    else if (strcmp(tokenName, "O_LESS_EQ") == 0)
        tokens[tokenCount].tokenValue = O_LESS_EQ;
    else if (strcmp(tokenName, "O_GREATER_EQ") == 0)
        tokens[tokenCount].tokenValue = O_GREATER_EQ;
    else if (strcmp(tokenName, "O_AND") == 0)
        tokens[tokenCount].tokenValue = O_AND;
    else if (strcmp(tokenName, "O_OR") == 0)
        tokens[tokenCount].tokenValue = O_OR;
    else if (strcmp(tokenName, "O_NOT") == 0)
        tokens[tokenCount].tokenValue = O_NOT;
    else if (strcmp(tokenName, "O_MODULO") == 0)
        tokens[tokenCount].tokenValue = O_MODULO;   

    else {
        syntaxError("Unknown token", 0, lexeme); // token not recognized
    }

    tokens[tokenCount].category = CAT_UNKNOWN;
}


            tokenCount++;
        }
    }

    fclose(file);
    printf("Loaded %d tokens from %s\n", tokenCount, filename);
}


// Utility Functions

Token getCurrentToken() {
    return tokens[currentToken];
}

int check(int expected) {
    int result = tokens[currentToken].tokenValue == expected;
    return result;
}


void match(int expected) {
    if (tokens[currentToken].tokenValue == expected) {
        currentToken++;
    } else {
        syntaxError("Mismatched expected token", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
    }
}

// Grammar Implementation (Bottom-Up Order)

void parseRelOp() {
    if (check(O_EQUAL) || check(O_NOT_EQUAL) || check(O_GREATER) || check(O_LESS) ||
        check(O_GREATER_EQ) || check(O_LESS_EQ))
        currentToken++;
    else {
        syntaxError("Expected relational operator", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
    }
}


void parseFactor() {

    if (check(L_IDENTIFIER) || check(L_BILANG_LITERAL) || check(L_KWERDAS_LITERAL) || check(L_BULYAN_LITERAL) || check(L_LUTANG_LITERAL))
        currentToken++;
    else if (check(D_LPAREN)) {
        match(D_LPAREN);
        parseExpression();
        match(D_RPAREN);
    } else {
        syntaxError("Unexpected factor", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
    }
}

// Operator precedence
void parseTermTail() {
    while (check(O_MULTIPLY) || check(O_DIVIDE)) {
        currentToken++;
        parseFactor();
    }
}

void parseTerm() {
    parseFactor();
    parseTermTail();
}

void parseExpressionTail() {
    while (check(O_PLUS) || check(O_MINUS)) {
        currentToken++;
        parseTerm();
    }
}

void parseExpression() {
    parseTerm();
    parseExpressionTail();
}


void parseBooleanExpression() {
    parseExpression();
    parseRelOp();
    parseExpression();
}

void parseAssignmentStatement() {
    // Match the variable
    match(L_IDENTIFIER);

    // Match '='
    match(O_ASSIGN);

    // Only parse an expression if the next token is valid for an expression
    if (check(L_BILANG_LITERAL) || check(L_LUTANG_LITERAL) || 
        check(L_KWERDAS_LITERAL) || check(L_BULYAN_LITERAL) || 
        check(L_IDENTIFIER) || check(D_LPAREN)) 
    {
        parseExpression();
    } 
    else {
        syntaxError("Expected expression after '='", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
    }

    // Match semicolon
    match(D_SEMICOLON);
}




void parseDeclarationStatement() {
    // Match data type
    if (check(R_BILANG) || check(R_LUTANG) || check(R_BULYAN) || check(R_KWERDAS)) {
        currentToken++;
    } else {
        syntaxError("Expected data type", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
    }

    // Match variable name
    match(L_IDENTIFIER);

    // Optional array brackets: array_name[size]
    if (check(D_LBRACKET)) {
        match(D_LBRACKET);
        if (check(L_BILANG_LITERAL)) {
            match(L_BILANG_LITERAL);  // Array size
        } else {
            syntaxError("Expected array size\n", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
        }
        match(D_RBRACKET);
    }

    // Optional initialization: '=' followed by expression
    if (check(O_ASSIGN)) {
        match(O_ASSIGN);

        if (check(L_BILANG_LITERAL) || check(L_LUTANG_LITERAL) || 
            check(L_KWERDAS_LITERAL) || check(L_BULYAN_LITERAL) || 
            check(L_IDENTIFIER) || check(D_LPAREN)) 
        {
            parseExpression();
        } else {
            syntaxError("Expected expression after '='\n", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
        }
    }

    // Handle multiple declarations separated by commas
    while (check(D_COMMA)) {
        match(D_COMMA);
        match(L_IDENTIFIER);

        // Optional array brackets
        if (check(D_LBRACKET)) {
            match(D_LBRACKET);
            if (check(L_BILANG_LITERAL)) match(L_BILANG_LITERAL);
            match(D_RBRACKET);
        }

        // Optional initialization
        if (check(O_ASSIGN)) {
            match(O_ASSIGN);

            if (check(L_BILANG_LITERAL) || check(L_LUTANG_LITERAL) || 
                check(L_KWERDAS_LITERAL) || check(L_BULYAN_LITERAL) || 
                check(L_IDENTIFIER) || check(D_LPAREN)) 
            {
                parseExpression();
            } else {
                syntaxError("Expected expression after '='\n", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
            }
        }
    }

    // Match semicolon at the end
    match(D_SEMICOLON);
}


void parseLoopStatement() {
    if (check(K_PARA)) {
        match(K_PARA);
        match(D_LPAREN);
        
        // First part: initialization (already has semicolon)
        parseAssignmentStatement();
        
        // Second part: condition
        parseBooleanExpression();
        match(D_SEMICOLON);
        
        // Third part: increment (No semicolon inside for loop header)
        match(L_IDENTIFIER);
        match(O_ASSIGN);
        parseExpression();
        // No semicolon here!
        
        match(D_RPAREN);
        match(D_LBRACE);
        parseStatementList();
        match(D_RBRACE);
        
    } else if (check(K_HABANG)) {
        match(K_HABANG);
        match(D_LPAREN);
        parseBooleanExpression();
        match(D_RPAREN);
        match(D_LBRACE);
        parseStatementList();
        match(D_RBRACE);
        
    } else if (check(K_GAWIN)) {
        match(K_GAWIN);
        match(D_LBRACE);
        parseStatementList();
        match(D_RBRACE);
        match(K_HABANG);
        match(D_LPAREN);
        parseBooleanExpression();
        match(D_RPAREN);
        match(D_SEMICOLON);
    }
}

void parseConditionalStatement() {
    match(K_KUNG);
    match(D_LPAREN);
    parseBooleanExpression();
    match(D_RPAREN);
    match(D_LBRACE);
    parseStatementList();
    match(D_RBRACE);

    if (check(K_KUNDIMAN)) {
        match(K_KUNDIMAN);
        match(D_LPAREN);
        parseBooleanExpression();
        match(D_RPAREN);
        match(D_LBRACE);
        parseStatementList();
        match(D_RBRACE);
    }

    if (check(K_KUNDI)) {
        match(K_KUNDI);
        match(D_LBRACE);
        parseStatementList();
        match(D_RBRACE);
    }
}

void parseStatement() {
    if (check(R_BILANG) || check(R_LUTANG) || check(R_BULYAN) || check(R_KWERDAS))
        parseDeclarationStatement();
    else if (check(L_IDENTIFIER))
        parseAssignmentStatement();
    else if (check(K_KUNG))
        parseConditionalStatement();
    else if (check(K_PARA) || check(K_HABANG) || check(K_GAWIN))
        parseLoopStatement();
    else {
        syntaxError("Unexpected \n", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
    }
}

void parseStatementList() {
    while (check(R_BILANG) || check(R_LUTANG) || check(R_BULYAN) || check(R_KWERDAS) ||
           check(L_IDENTIFIER) || check(K_KUNG) || check(K_PARA) || check(K_HABANG) || check(K_GAWIN)) {
        parseStatement();
    }
}

void parseFunction() {
    match(R_WALA);
    match(R_UGAT);
    match(D_LPAREN);
    match(D_RPAREN);
    match(D_LBRACE);
    parseStatementList();
    match(D_RBRACE);
}

void parseFunctionList() {
    if (check(R_WALA)) {
        parseFunction();
        parseFunctionList();
    }
}

void parseProgram() {
    printf("Parsing Program...\n");
    parseFunctionList();

    if (currentToken < tokenCount)
        printf("Warning: Extra tokens after program end\n");
    else
        printf("Syntax Analysis Complete.\n");
}