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
    node->children=NULL;
    node->child_count=0;
    return node;
}
ASTNode* parse_primary(Parser *parser)
{
    if(parser->current_token.type==TOKEN_INT)
    {
        ASTNode *node=create_node(NODE_INT,parser->current_token.lexeme);
        advance(parser);
        return node;
    }
    if(parser->current_token.type==TOKEN_IDENTIFIER)
    {
        ASTNode *node=create_node(NODE_VAR,parser->current_token.lexeme);
        advance(parser);
        return node;
    }
    return NULL;
}
ASTNode* parse_term(Parser *parser)
{
    ASTNode *left=parse_primary(parser);
    while(parser->current_token.type==TOKEN_MUL || parser->current_token.type==TOKEN_DIV)
    {
        TokenType op=parser->current_token.type;
        advance(parser);
        NodeType node_type=(op==TOKEN_MUL) ? NODE_MUL : NODE_DIV;
        const char* op_str=(op==TOKEN_MUL) ? "*" : "/";
        ASTNode *new_node=create_node(node_type,op_str);
        new_node->left=left;
        new_node->right=parse_primary(parser);
        left=new_node;
    }
    return left;
}
ASTNode* parse_expression(Parser *parser)
{
    ASTNode *left=parse_term(parser);
    while(parser->current_token.type==TOKEN_PLUS||parser->current_token.type==TOKEN_MINUS)
    {
        TokenType op=parser->current_token.type;
        advance(parser);
        NodeType node_type=(op==TOKEN_PLUS) ? NODE_ADD : NODE_SUB;
        const char* op_str=(op==TOKEN_PLUS) ? "+" : "-";
        ASTNode *new_node=create_node(node_type,op_str);
        new_node->left=left;
        new_node->right=parse_term(parser);
        left=new_node;
    }
    return left;
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
            ASTNode *expr=parse_expression(parser);
            ASTNode *var=create_node(NODE_VAR,var_name);
            ASTNode *assign=create_node(NODE_ASSIGN,"=");
            assign->left=var;
            assign->right=expr;
            return assign;
        }
    }
    return NULL;
}
void add_child(ASTNode *parent,ASTNode *child)
{
    if(!parent||!child)
    {
        return;
    }
    parent->children=realloc(parent->children,sizeof(ASTNode*) * (parent->child_count + 1));
    parent->children[parent->child_count]=child;
    parent->child_count++;
}
ASTNode* parse_program(Parser *parser)
{
    ASTNode *block=create_node(NODE_BLOCK,"BLOCK");
    while(parser->current_token.type!=TOKEN_EOF)
    {
        ASTNode *stmt=parse_statement(parser);
        if(stmt)
        {
            add_child(block,stmt);
        }
    }
    return block;
}
