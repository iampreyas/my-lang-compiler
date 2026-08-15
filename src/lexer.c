#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "../include/lexer.h"

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

static char peek_next(Lexer *lexer)
{
    if(lexer->source[lexer->cursor]=='\0')
    {
        return '\0';
    }
    return lexer->source[lexer->cursor+1];
}

static char advance_char(Lexer *lexer)
{
    char c=lexer->source[lexer->cursor];
    lexer->cursor++;
    if(c=='\n')
    {
        lexer->line++;
    }
    return c;
}

static void skip_whitespace_and_comments(Lexer *lexer)
{
    for(;;)
    {
        char c=peek(lexer);
        if(c==' '||c=='\t'||c=='\r'||c=='\n')
        {
            advance_char(lexer);
        }
        else if(c=='/' && peek_next(lexer)=='/')
        {
            while(peek(lexer)!='\n' && peek(lexer)!='\0')
            {
                advance_char(lexer);
            }
        }
        else
        {
            break;
        }
    }
}

static Token make_token(TokenType type,const char *lexeme,int line)
{
    Token token;
    token.type=type;
    token.line=line;
    if(lexeme)
    {
        strncpy(token.lexeme,lexeme,sizeof(token.lexeme)-1);
        token.lexeme[sizeof(token.lexeme)-1]='\0';
    }
    else
    {
        token.lexeme[0]='\0';
    }
    return token;
}

static Token read_number(Lexer *lexer)
{
    int start=lexer->cursor;
    int line=lexer->line;
    while(isdigit((unsigned char)peek(lexer)))
    {
        advance_char(lexer);
    }
    int len=lexer->cursor-start;
    char buffer[64];
    if(len>=(int)sizeof(buffer))
    {
        len=sizeof(buffer)-1;
    }
    strncpy(buffer,lexer->source+start,len);
    buffer[len]='\0';
    return make_token(TOKEN_INT,buffer,line);
}

static Token read_identifier(Lexer *lexer)
{
    int start=lexer->cursor;
    int line=lexer->line;
    while(isalnum((unsigned char)peek(lexer)) || peek(lexer)=='_')
    {
        advance_char(lexer);
    }
    int len=lexer->cursor-start;
    char buffer[64];
    if(len>=(int)sizeof(buffer))
    {
        len=sizeof(buffer)-1;
    }
    strncpy(buffer,lexer->source+start,len);
    buffer[len]='\0';

    if(strcmp(buffer,"print")==0)
    {
        return make_token(TOKEN_PRINT,buffer,line);
    }
    if(strcmp(buffer,"if")==0)
    {
        return make_token(TOKEN_IF,buffer,line);
    }
    if(strcmp(buffer,"else")==0)
    {
        return make_token(TOKEN_ELSE,buffer,line);
    }
    return make_token(TOKEN_IDENTIFIER,buffer,line);
}

static Token read_string(Lexer *lexer)
{
    int line=lexer->line;
    advance_char(lexer); /* skip opening quote */
    int start=lexer->cursor;
    while(peek(lexer)!='"' && peek(lexer)!='\0')
    {
        advance_char(lexer);
    }
    int len=lexer->cursor-start;
    char buffer[64];
    if(len>=(int)sizeof(buffer))
    {
        len=sizeof(buffer)-1;
    }
    strncpy(buffer,lexer->source+start,len);
    buffer[len]='\0';
    if(peek(lexer)=='"')
    {
        advance_char(lexer); /* skip closing quote */
    }
    else
    {
        printf("[ERROR] Unterminated string on line %d\n",line);
    }
    return make_token(TOKEN_STRING,buffer,line);
}

Token lexer_next_token(Lexer *lexer)
{
    skip_whitespace_and_comments(lexer);

    int line=lexer->line;
    char c=peek(lexer);

    if(c=='\0')
    {
        return make_token(TOKEN_EOF,NULL,line);
    }

    if(isdigit((unsigned char)c))
    {
        return read_number(lexer);
    }

    if(isalpha((unsigned char)c) || c=='_')
    {
        return read_identifier(lexer);
    }

    if(c=='"')
    {
        return read_string(lexer);
    }

    /* two-character operators first */
    if(c=='=' && peek_next(lexer)=='=')
    {
        advance_char(lexer);
        advance_char(lexer);
        return make_token(TOKEN_EQ,"==",line);
    }
    if(c=='!' && peek_next(lexer)=='=')
    {
        advance_char(lexer);
        advance_char(lexer);
        return make_token(TOKEN_NEQ,"!=",line);
    }

    advance_char(lexer);
    switch(c)
    {
        case '=': return make_token(TOKEN_ASSIGN,"=",line);
        case '+': return make_token(TOKEN_PLUS,"+",line);
        case '-': return make_token(TOKEN_MINUS,"-",line);
        case '*': return make_token(TOKEN_MUL,"*",line);
        case '/': return make_token(TOKEN_DIV,"/",line);
        case '<': return make_token(TOKEN_LT,"<",line);
        case '>': return make_token(TOKEN_GT,">",line);
        case '(': return make_token(TOKEN_LPAREN,"(",line);
        case ')': return make_token(TOKEN_RPAREN,")",line);
        case '[': return make_token(TOKEN_LBRACKET,"[",line);
        case ']': return make_token(TOKEN_RBRACKET,"]",line);
        case ',': return make_token(TOKEN_COMMA,",",line);
        default:
        {
            char unknown[2]={c,'\0'};
            printf("[ERROR] Unknown character '%c' on line %d\n",c,line);
            return make_token(TOKEN_UNKNOWN,unknown,line);
        }
    }
}