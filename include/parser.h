#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
typedef enum
{
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_INT,
    NODE_VAR,
    NODE_BLOCK,
    NODE_STRING,
    NODE_BINARY_OP,
    NODE_LT,
    NODE_GT,
    NODE_EQ,
    NODE_NEQ,
    NODE_IF,
    NODE_WHILE,
    NODE_BREAK,
    NODE_NEGATE,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV
}NodeType;
typedef struct ASTNode
{
    NodeType type;
    char name[100];
    int value;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode **children;
    int child_count;
}ASTNode;
typedef struct
{
    Lexer *lexer;
    Token current_token;
}Parser;
void parser_init(Parser *parser,Lexer *lexer);
ASTNode* parse_statement(Parser *parser);
ASTNode* parse_program(Parser *parser);
ASTNode* parse_comparison(Parser *parser);
ASTNode* parse_factor(Parser *parser);
#endif