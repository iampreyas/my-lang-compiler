#ifndef EVAL_H
#define EVAL_H
#include "parser.h"
typedef struct
{
    char name[64];
    int value;
}variable;
typedef struct 
{
    variable vars[100];
    int count;
}environment;
void env_init(environment *env);
int eval(ASTNode *node,environment *env);
#endif
