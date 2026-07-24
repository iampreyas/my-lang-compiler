#ifndef TOKEN_H
#define TOKEN_H
typedef enum
{
    TOKEN_EOF,
    TOKEN_INT,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_COMMA,
    TOKEN_PRINT,
    TOKEN_UNKNOWN
}TokenType;
typedef struct
{
    TokenType type;
    char lexeme[64];
    int line;
}Token;
#endif