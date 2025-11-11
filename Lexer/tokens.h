#ifndef TOKENS_H
#define TOKENS_H
//Category
typedef enum {
    CAT_KEYWORD,
    CAT_RESERVED,
    CAT_NOISEWORD,
    CAT_OPERATOR,
    CAT_DELIMITER,
    CAT_LITERAL,
    CAT_COMMENT,
    CAT_UNKNOWN
} TokenCategory;


//Tokens for keywords category:
typedef enum {
    K_ANI = 0,
    K_TANIM,
    K_PARA,
    K_HABANG,
    K_KUNG,
    K_KUNDI,
    K_KUNDIMAN,
    K_GAWIN,
    K_TIBAG,
    K_TULOY,
    K_PANGKAT,
    K_STATIK,
    K_PRIBADO,
    K_PROTEKTADO,
    K_PUBLIKO,

    R_TAMA,
    R_MALI,
    R_UGAT,    
    R_BALIK, 
    R_BILANG,
    R_KWERDAS,
    R_TITIK,
    R_LUTANG,
    R_BULYAN,
    R_DOBLE,
    R_WALA,
    R_PI,            
    R_E_NUM,                
    R_SAMPLE_CONST_STRING,
    R_Kiss,

    N_NG,
    N_AY,
    N_BUNGA,
    N_WAKAS,
    N_SA,
    N_ANG,
    N_MULA,
    N_ITAKDA,

    O_PLUS,        // +
    O_MINUS,       // -
    O_MULTIPLY,    // *
    O_DIVIDE,      // /
    O_POW,        // ^
    O_MODULO,      // %
    O_ASSIGN,      // =
    O_EQUAL,       // ==
    O_NOT_EQUAL,   // !=
    O_LESS,        // <
    O_GREATER,     // >
    O_LESS_EQ,     // <=
    O_GREATER_EQ,  // >=
    O_AND,         // &&
    O_OR,          // ||
    O_NOT,

    D_LPAREN,      // (
    D_RPAREN,      // )
    D_LBRACE,      // {
    D_RBRACE,      // }
    D_LBRACKET,    // [
    D_RBRACKET,    // ]
    D_COMMA,       // ,
    D_SEMICOLON,   // ;
    D_COLON,       // :
    D_DOT,         // .
    D_QUOTE,       // "
    D_SQUOTE,

    L_IDENTIFIER,
    L_BILANG_LITERAL,
    L_LUTANG_LITERAL,
    L_KWERDAS_LITERAL,
    L_TITIK_LITERAL,
    L_BULYAN_LITERAL,  // (Tama, Mali)

    C_SINGLE_LINE,  // //
    C_MULTI_LINE    // /* */
} TokenType;

// Keep old typedefs for backward compatibility if needed
typedef TokenType KeywordToken;
typedef TokenType ReservedToken;
typedef TokenType NoiseWordToken;
typedef TokenType OperatorToken;
typedef TokenType DelimiterToken;
typedef TokenType LiteralToken;
typedef TokenType CommentToken;

//General structure for a Token
typedef struct {
    TokenCategory category;
    int tokenValue;
    char* lexeme;
    int lineNumber;
} Token;

#endif