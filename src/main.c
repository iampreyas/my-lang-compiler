#include<stdio.h>
#include<stdlib.h>
#include "../include/parser.h"
#include "../include/lexer.h"
#include "../include/eval.h"
char* read_file(const char *filename)
{
    FILE *file=fopen(filename,"rb");
    if (!file)
    {
        printf("[ERROR] File '%s' not founded\n",filename);
        return NULL;
    }
    fseek(file,0,SEEK_END);
    long size=ftell(file);
    fseek(file,0,SEEK_SET);
    char *buffer=(char *)malloc(size+1);
    if (!buffer)
    {
        printf("[ERROR] Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    fread(buffer,1,size,file);
    buffer[size]='\0';
    fclose(file);
    return buffer;
}
void print_ast(ASTNode *node,int level)
{
    if(!node)
    {
        return;
    }
    for(int i=0;i<level;i++)
    {
        printf(" ");
    }
    printf("Node: %s (Type: %d)\n",node->name,node->type);
    if(node->type==NODE_BLOCK)
    {
        for(int i=0;i<node->child_count;i++)
        {
            print_ast(node->children[i],level + 1);
        }
    }
    else
    {
        print_ast(node->left,level+1);
        print_ast(node->right,level+1);
    }
}
int main(int argc,char *argv[])
{
    printf("Started\n");
    fflush(stdout);

    if(argc<2)
    {
        printf("Usage: %s <filename.cris>\n",argv[0]);
        return 1;
    }
    char *code=read_file(argv[1]);
    if(!code)
    {
        return 1;
    }
    Lexer lexer;
    lexer_init(&lexer,code);
    Parser parser;
    parser_init(&parser,&lexer);
    ASTNode *root=parse_program(&parser);
    if(root)
    {
        printf("---AST Tree Generated Successfully---\n");
        print_ast(root,0);
        printf("\n---Running Evaluator---\n");
        environment env;
        env_init(&env);
        int result=eval(root,&env);
        printf("Final Result: %d\n",result);
        fflush(stdout);
    }
    else
    {
        printf("Parsing error!\n");
    }
    free(code);
    return 0;
}