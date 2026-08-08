#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
typedef enum
{
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_INT,
    NODE_VAR,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV
}NodeType;
typedef struct ASTNode
{
    NodeType type;
    char name[64];
    struct ASTNode *left;
    struct ASTNode *right;
}ASTNode;
typedef struct
{
    Lexer *lexer;
    Token current_token;
}Parser;
void parser_init(Parser *parser,Lexer *lexer);
ASTNode* parse_statement(Parser *parser);
#endif