#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
typedef struct
{
    char name[50];
    int value;
}Variable;
Variable symbol_table[100];
int variable_count=0;
void set_variable(char *name,int val)
{
    for(int i=0;i<variable_count;i++)
    {
        if(strcmp(symbol_table[i].name,name)==0)
        {
            symbol_table[i].value=val;
            return;
        }
    }
    strcpy(symbol_table[variable_count].name,name);
    symbol_table[variable_count].value=val;
    variable_count++;
}
int get_variable(char *name,int *found)
{
    for(int i=0;i<variable_count;i++)
    {
        if(strcmp(symbol_table[i].name,name)==0)
        {
            *found=1;
            return symbol_table[i].value;
        }
    }
    *found=0;
    return 0;
}

int main()
{
    FILE *file=fopen("compiler/examples/hello.cris","r");
    if(file==NULL)
    {
        printf("Error\n");
        return 1;
    }
    printf("hello.cris opened successfully!\n");
    char line[100];
    int execute_next_line=1;
    while(fgets(line,sizeof(line),file)!=NULL)
    {
        line[strcspn(line, "\r\n")]=0;
        if(!execute_next_line)
        {
            execute_next_line=1;
            continue;
        }
        if(line[0]=='#')
        {
            continue;
        }
        if(strlen(line)==0||line[0]=='\0')
        {
            continue;
        }
        printf("LINE:[%s]",line);
        if(strncmp(line,"if",2)==0)
        {
            char arg1[50]={0},arg2[50]={0},comp_op=0;
            if(sscanf(line+3,"%49[^<>=]%c%49s",arg1,&comp_op,arg2)==3)
            {
                int is_var1=0,is_var2=0;
                int val=get_variable(arg1,&is_var1);
                if(!is_var1)
                {
                    val=atoi(arg1);
                }
                int val2=get_variable(arg2,&is_var2);
                if(!is_var2)
                {
                    val2=atoi(arg2);
                }
                if(comp_op=='>' && !(val>val2))
                {
                    execute_next_line=0;
                }
                else if(comp_op=='<' && !(val<val2))
                {
                    execute_next_line=0;
                }
                else if(comp_op=='=' && !(val==val2))
                {
                    execute_next_line=0;
                }
                else if(comp_op=='!' && (val==val2))
                {
                    execute_next_line=0;
                }
            }
            continue;
        }
        else if(strncmp(line,"print",5)==0)
        {
            char format_string[100]={0};
            char var_name[50]={0};
            if(sscanf(line+6,"%[^,],%s",format_string,var_name)==2)
            {
                int is_variable=0;
                int val=get_variable(var_name,&is_variable);
                if(is_variable)
                {
                    printf(format_string,val);
                    printf("\n");
                    continue;
                }
            }
            printf("%s\n",line+6);
            continue;
        }
        else if(strchr(line,'=')!=NULL)
        {
            char var_name[50]={0};
            char arg1[50]={0};
            char arg2[50]={0};
            char op=0;
            int var_value;
            if(sscanf(line,"%49[^=]=%[^+^-]%c%s",var_name,arg1,&op,arg2)==4)
            {
                printf("var_name='%s'\n",var_name);
                printf("arg1='%s'\n",arg1);
                printf("op='%c'\n",op);
                printf("arg1='%s'\n",arg1);
                printf("arg2='%s'\n",arg2);
                int is_var1=0,is_var2=0;
                int val1=get_variable(arg1,&is_var1);
                if(!is_var1) 
                {
                    val1=atoi(arg1);
                }
                int val2=get_variable(arg2,&is_var2);
                if(!is_var2)
                {
                     val2=atoi(arg2);
                }
                printf("val1=%d\n",val1);
                printf("val2=%d\n",val2);
                int final_val=0;
                if(op=='+') 
                {
                    final_val=val1+val2;
                }
                else if(op=='-')
                {
                     final_val=val1-val2;
                }
                set_variable(var_name,final_val);
                printf("[LOG]: Math Calculation %s stored with value %d\n",var_name,final_val);
            }
            else
            {
                int var_value=0;
                if(sscanf(line,"%[^=]=%d",var_name,&var_value)==2)
                {
                    set_variable(var_name,var_value);
                    printf("[LOG]:Variable %s stored With value %d\n",var_name,var_value);
                }
            }
        }
    }
    fclose(file);
    return 0;
}