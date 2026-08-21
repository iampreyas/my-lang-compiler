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
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_BREAK,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_BANG,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_COMMA,
    TOKEN_PRINT,
    TOKEN_STRING,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_WHILE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_UNKNOWN,
}TokenType;
typedef struct
{
    TokenType type;
    char lexeme[64];
    int line;
}Token;
#endif