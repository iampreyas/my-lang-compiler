#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../include/eval.h"
void env_init(environment *env)
{
    env->count=0;
}
static void set_var(environment *env,const char *name,int value)
{
    for(int i=0;i<env->count;i++)
    {
        if(strcmp(env->vars[i].name,name)==0)
        {
            env->vars[i].value=value;
            return;
        }
    }
    strcpy(env->vars[env->count].name,name);
    env->vars[env->count].value=value;
    env->count++;
}
int eval(ASTNode *node,environment *env)
{
    if(!node)
    {
        return 0;
    }
    if(node->type==NODE_INT)
    {
        return atoi(node->name);
    }
    if(node->type==NODE_ADD)
    {
        return eval(node->left,env) + eval(node->right,env);
    }
    if(node->type==NODE_SUB)
    {
        return eval(node->left,env) - eval(node->right,env);
    }
    if(node->type==NODE_MUL)
    {
        return eval(node->left,env) * eval(node->right,env);
    }
    if(node->type==NODE_DIV)
    {
        int right_val=eval(node->right,env);
        if(right_val==0)
        {
            printf("[ERROR] Division by zero\n");
            exit(1);
        }
        return eval(node->left,env) / right_val;
    }
    if(node->type==NODE_ASSIGN)
    {
        int val=eval(node->right,env);
        set_var(env,node->left->name,val);
        printf("[SUCCESS] variable '%s' evaluated and set to %d\n",node->left->name,val);
        return val;
    }
    return 0;
}
