#include "parser.h"
#include "../Lexer/wordhash.h"

Token tokens[1000];
int tokenCount = 0;
int currentToken = 0;

/* ------------------ Helper: Map Token Names to Values ------------------ */

int getTokenValueFromName(const char *tokenName) {
    // Reserved words
    if (strcmp(tokenName, "R_WALA") == 0) return R_WALA;
    if (strcmp(tokenName, "R_UGAT") == 0) return R_UGAT;
    if (strcmp(tokenName, "R_BILANG") == 0) return R_BILANG;
    if (strcmp(tokenName, "R_LUTANG") == 0) return R_LUTANG;
    if (strcmp(tokenName, "R_BULYAN") == 0) return R_BULYAN;
    if (strcmp(tokenName, "R_KWERDAS") == 0) return R_KWERDAS;
    
    // Delimiters
    if (strcmp(tokenName, "D_LPAREN") == 0) return D_LPAREN;
    if (strcmp(tokenName, "D_RPAREN") == 0) return D_RPAREN;
    if (strcmp(tokenName, "D_LBRACE") == 0) return D_LBRACE;
    if (strcmp(tokenName, "D_RBRACE") == 0) return D_RBRACE;
    if (strcmp(tokenName, "D_SEMICOLON") == 0) return D_SEMICOLON;
    if (strcmp(tokenName, "D_COMMA") == 0) return D_COMMA;
    if (strcmp(tokenName, "D_LBRACKET") == 0) return D_LBRACKET;
    if (strcmp(tokenName, "D_RBRACKET") == 0) return D_RBRACKET;
    
    // Operators
    if (strcmp(tokenName, "O_ASSIGN") == 0) return O_ASSIGN;
    if (strcmp(tokenName, "O_PLUS") == 0) return O_PLUS;
    if (strcmp(tokenName, "O_MINUS") == 0) return O_MINUS;
    if (strcmp(tokenName, "O_MULTIPLY") == 0) return O_MULTIPLY;
    if (strcmp(tokenName, "O_DIVIDE") == 0) return O_DIVIDE;
    if (strcmp(tokenName, "O_EQUAL") == 0) return O_EQUAL;
    if (strcmp(tokenName, "O_NOT_EQUAL") == 0) return O_NOT_EQUAL;
    if (strcmp(tokenName, "O_GREATER") == 0) return O_GREATER;
    if (strcmp(tokenName, "O_LESS") == 0) return O_LESS;
    if (strcmp(tokenName, "O_GREATER_EQ") == 0) return O_GREATER_EQ;
    if (strcmp(tokenName, "O_LESS_EQ") == 0) return O_LESS_EQ;
    
    // Keywords
    if (strcmp(tokenName, "K_ANI") == 0) return K_ANI;
    if (strcmp(tokenName, "K_TANIM") == 0) return K_TANIM;
    if (strcmp(tokenName, "K_KUNG") == 0) return K_KUNG;
    if (strcmp(tokenName, "K_KUNDI") == 0) return K_KUNDI;
    if (strcmp(tokenName, "K_KUNDIMAN") == 0) return K_KUNDIMAN;
    if (strcmp(tokenName, "K_PARA") == 0) return K_PARA;
    if (strcmp(tokenName, "K_HABANG") == 0) return K_HABANG;
    if (strcmp(tokenName, "K_GAWIN") == 0) return K_GAWIN;
    
    // Literals/Identifiers
    if (strcmp(tokenName, "L_IDENTIFIER") == 0) return L_IDENTIFIER;
    if (strcmp(tokenName, "L_BILANG_LITERAL") == 0) return L_BILANG_LITERAL;
    if (strcmp(tokenName, "L_LUTANG_LITERAL") == 0) return L_LUTANG_LITERAL;
    if (strcmp(tokenName, "L_KWERDAS_LITERAL") == 0) return L_KWERDAS_LITERAL;
    if (strcmp(tokenName, "L_BULYAN_LITERAL") == 0) return L_BULYAN_LITERAL;
    
    return 0;  // Unknown token
}

/* ------------------ Token Loading ------------------ */

void loadTokensFromFile(const char *filename) {
    initialize_table();
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open %s\n", filename);
        exit(1);
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Lexeme") || strstr(line, "Token Name") || strlen(line) < 3)
            continue;
            
        char lexeme[100], tokenName[100];
        if (sscanf(line, "%s | %s", lexeme, tokenName) == 2) {
            // Skip comments - don't add them to token array
            if (strcmp(tokenName, "C_SINGLE_LINE") == 0 || 
                strcmp(tokenName, "C_MULTI_LINE") == 0) {
                continue;
            }
            
            tokens[tokenCount].lexeme = strdup(lexeme);
            tokens[tokenCount].lineNumber = tokenCount + 1;
            
            int category, value;
            if (hashLookup(lexeme, &category, &value)) {
                tokens[tokenCount].tokenValue = value;
            } else {
                tokens[tokenCount].tokenValue = getTokenValueFromName(tokenName);
            }
            
            tokenCount++;
        }
    }
    fclose(file);
    printf("Loaded %d tokens from %s\n", tokenCount, filename);
}

/* ------------------ Utility Functions ------------------ */

Token getCurrentToken() {
    return tokens[currentToken];
}

int check(int expected) {
    return tokens[currentToken].tokenValue == expected;
}

void match(int expected) {
    if (tokens[currentToken].tokenValue == expected) {
        currentToken++;
    } else {
        printf("Syntax Error at line %d: Expected token %d but found '%s'\n",
               tokens[currentToken].lineNumber,
               expected,
               tokens[currentToken].lexeme);
        exit(1);
    }
}

/* ------------------ Grammar Implementation (Bottom-Up Order) ------------------ */

void parseRelOp() {
    if (check(O_EQUAL) || check(O_NOT_EQUAL) || check(O_GREATER) || check(O_LESS) ||
        check(O_GREATER_EQ) || check(O_LESS_EQ))
        currentToken++;
    else {
        printf("Syntax Error: Expected relational operator at '%s'\n", tokens[currentToken].lexeme);
        exit(1);
    }
}

void parseFactor() {

    printf("parseFactor() checking token: '%s' with value %d\n", 
           tokens[currentToken].lexeme, 
           tokens[currentToken].tokenValue);

    if (check(L_IDENTIFIER) || check(L_BILANG_LITERAL) || check(L_KWERDAS_LITERAL) || check(L_BULYAN_LITERAL) || check(L_LUTANG_LITERAL))
        currentToken++;
    else if (check(D_LPAREN)) {
        match(D_LPAREN);
        parseExpression();
        match(D_RPAREN);
    } else {
        printf("Syntax Error at line '%d': Unexpected factor '%s'\n", tokens[currentToken].lineNumber, tokens[currentToken].lexeme);
        exit(1);
    }
}

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
    match(L_IDENTIFIER);
    match(O_ASSIGN);
    parseExpression();
    match(D_SEMICOLON);
}



void parseDeclarationStatement() {
    // Match data type
    if (check(R_BILANG) || check(R_LUTANG) || check(R_BULYAN) || check(R_KWERDAS))
        currentToken++;
    else {
        printf("Syntax Error: Expected data type\n");
        exit(1);
    }

    // Match identifier
    match(L_IDENTIFIER);
    
    // Optional array brackets: [size]
    if (check(D_LBRACKET)) {
        match(D_LBRACKET);
        if (check(L_BILANG_LITERAL)) {
            match(L_BILANG_LITERAL);  // Array size
        }
        match(D_RBRACKET);
    }
    
    // Optional initialization: = expression
    if (check(O_ASSIGN)) {
        match(O_ASSIGN);
        parseExpression();
    }
    
    // Handle multiple declarations: , identifier [size] = expression
    while (check(D_COMMA)) {
        match(D_COMMA);
        match(L_IDENTIFIER);
        
        // Optional array brackets
        if (check(D_LBRACKET)) {
            match(D_LBRACKET);
            if (check(L_BILANG_LITERAL)) {
                match(L_BILANG_LITERAL);
            }
            match(D_RBRACKET);
        }
        
        // Optional initialization
        if (check(O_ASSIGN)) {
            match(O_ASSIGN);
            parseExpression();
        }
    }
    
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
        
        // Third part: increment (NO semicolon inside for loop header)
        match(L_IDENTIFIER);
        match(O_ASSIGN);
        parseExpression();
        // NO semicolon here!
        
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
        printf("Syntax Error at line %d: Unexpected '%s'\n",
               tokens[currentToken].lineNumber,
               tokens[currentToken].lexeme);
        exit(1);
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