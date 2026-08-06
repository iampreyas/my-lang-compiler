#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../include/parser.h"
static void advance(Parser *parser)
{
    parser->current_token=lexer_next_token(parser->lexer);
}
void parser_init(Parser *parser,Lexer *lexer)
{
    parser->lexer=lexer;
    advance(parser);
}
ASTNode* create_node(NodeType type,const char* name)
{
    ASTNode *node=(ASTNode*)malloc(sizeof(ASTNode));
    node->type=type;
    if(name)
    {
        strcpy(node->name,name);
    }
    else
    {
        node->name[0]='\0';
    }
    node->left=NULL;
    node->right=NULL;
    return node;
}
ASTNode* parse_statement(Parser *parser)
{
    if(parser->current_token.type==TOKEN_IDENTIFIER)
    {
        char var_name[64];
        strcpy(var_name,parser->current_token.lexeme);
        advance(parser);
        if(parser->current_token.type==TOKEN_ASSIGN)
        {
            advance(parser);
            if(parser->current_token.type==TOKEN_INT)
            {
                ASTNode *var=create_node(NODE_VAR,var_name);
                ASTNode *val=create_node(NODE_INT,parser->current_token.lexeme);
                ASTNode *assign=create_node(NODE_ASSIGN,"=");
                assign->left=var;
                assign->right=val;
                advance(parser);
                return assign;
            }
        }
    }
    return NULL;
}