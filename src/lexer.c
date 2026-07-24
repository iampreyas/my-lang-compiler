#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "lexer.h"
void lexer_init(Lexer *lexer,const char *source)
{
    lexer->source=source;
    lexer->cursor=0;
    lexer->line=1;
}
static char peek(Lexer *lexer)
{
    return lexer->source[lexer->cursor];
}
static char advance(Lexer *lexer)
{
    return lexer->source[lexer->cursor++];
}
static void skip_whitespace(Lexer *lexer)
{
    while(peek(lexer)==' '||peek(lexer)=='\t'||peek(lexer)=='\r'||peek(lexer)=='\n')
    {
        if(peek(lexer)=='\n')
        {
            lexer->line++;
        }
        advance(lexer);
    }
}
Token lexer_next_token(Lexer *lexer)
{
    skip_whitespace(lexer);
    char c=peek(lexer);
    if(c=='\0')
    {
        Token t={TOKEN_EOF,"EOF",lexer->line};
        return t;
    }
    if(isdigit(c))
    {
        Token t;
        t.type=TOKEN_INT;
        t.line=lexer->line;
        int i=0;
        while (isdigit(peek(lexer)) && i<63)
        {
            t.lexeme[i++]=advance(lexer);
        }
        t.lexeme[i]='\0';
        return t;
    }

    if(isalpha(c) || c == '_') 
    {
        Token t;
        t.line = lexer->line;
        int i = 0;
        while((isalnum(peek(lexer)) || peek(lexer) == '_') && i < 63)
        {
            t.lexeme[i++] = advance(lexer);
        }
        t.lexeme[i]='\0';
        if(strcmp(t.lexeme, "print") == 0)
        {
            t.type = TOKEN_PRINT;
        }
        else
        {
            t.type = TOKEN_IDENTIFIER;
        }
        return t;
    }
    advance(lexer);
    Token t;
    t.line=lexer->line;
    t.lexeme[0]=c;
    t.lexeme[1]='\0';
    switch (c)
    {
        case '=':
            t.type = TOKEN_ASSIGN;
            break;
        case '+':
            t.type = TOKEN_PLUS;
            break;
        case '-':
            t.type = TOKEN_MINUS;
            break;
        case '(':
            t.type = TOKEN_LPAREN;
            break;
        case ')':
            t.type = TOKEN_RPAREN;
            break;
        case '[':
            t.type = TOKEN_LBRACKET;
            break;
        case ']':
            t.type = TOKEN_RBRACKET;
            break;
        case ',':
            t.type = TOKEN_COMMA;
            break;
        default:
            t.type = TOKEN_UNKNOWN;
            break;
    }

    return t;
}