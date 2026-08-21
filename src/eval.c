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
static int get_var(environment *env, const char *name)
{
    for(int i=0;i<env->count;i++)
    {
        if(strcmp(env->vars[i].name,name)==0)
        {
            return env->vars[i].value;
        }
    }
    printf("[ERROR] Undefined variable '%s'\n", name);
    exit(1);
}
int eval(ASTNode *node,environment *env)
{
    if(!node)
    {
        return 0;
    }
    if(node->type==NODE_BLOCK)
    {
        int last_val=0;
        for(int i=0;i<node->child_count;i++)
        {
            last_val=eval(node->children[i],env);
            if(last_val==-999)
            {
                return -999;
            }
        }
        return last_val;
    }
    if(node->type==NODE_PRINT)
    {
        ASTNode *child=node->children[0];
        if(child->type==NODE_STRING)
        {
            printf("> %s\n",child->name);
        }
        else
        {
            int val=eval(node->children[0],env);
            printf("> %d\n",val);
            return val;
        }
    }
    if(node->type==NODE_STRING)
    {
        return 0;
    }
    if(node->type==NODE_VAR)
    {
        return get_var(env,node->name);
    }
    if(node->type==NODE_INT)
    {
        return atoi(node->name);
    }
    if(node->type==NODE_LT)
    {
        return eval(node->left,env) < eval(node->right,env);
    }
    if(node->type==NODE_GT)
    {
        return eval(node->left,env) > eval(node->right,env);
    }
    if(node->type==NODE_EQ)
    {
        return eval(node->left,env) == eval(node->right,env);
    }
    if(node->type==NODE_NEQ)
    {
        return eval(node->left,env) != eval(node->right,env);
    }
    if(node->type==NODE_NEGATE)
    {
        return -eval(node->children[0],env);
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
    if(node->type==NODE_BREAK)
    {
        return -999;
    }
    if(node->type==NODE_WHILE)
    {
        while(eval(node->children[0],env))
        {
           int res=eval(node->children[1],env);
           if(res==-999)
           {
            break;
           }
        }
        return 0;
    }
    if(node->type==NODE_IF)
    {
        int cond_value=eval(node->children[0],env);
        if(cond_value)
        {
            return eval(node->children[1],env);
        }
        else if(node->child_count>2)
        {
            return eval(node->children[2],env);
        }
        return 0;
    }
    if(node->type==NODE_NOT)
    {
        int val=eval(node->children[0],env);
        return !val;
    }
    if(node->type==NODE_AND)
    {
        int left=eval(node->left,env);
        if(!left)
        {
            return 0;
        }
        return eval(node->right,env);
    }
    if(node->type==NODE_OR)
    {
        int left=eval(node->left,env);
        if(left)
        {
            return 1;
        }
        return eval(node->right,env);
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
