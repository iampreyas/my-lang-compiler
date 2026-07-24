#include<stdio.h>
#include "../include/lexer.h"
int main()
{
    const char *code="x=10\n print x";
    Lexer lexer;
    lexer_init(&lexer,code);
    Token token;
    do
    {
        token=lexer_next_token(&lexer);
        printf("Line %d | Type: %d | Lexeme: '%s'\n",token.line,token.type,token.lexeme);
    } while (token.type != TOKEN_EOF);
    return 0;
}