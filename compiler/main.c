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
    while(fgets(line,sizeof(line),file)!=NULL)
    {
        printf("LINE:[%s]",line);
        if(strncmp(line,"print",5)==0)
        {
            printf("%s",line+6);
        }
        else if(strchr(line,'=')!=NULL)
        {
            char var_name[50];
            int var_value;
            if(sscanf(line,"%49[^=]=%d",var_name,&var_value)==2)
            {
                set_variable(var_name,var_value);
                printf("[LOG]:Variable %s stored With value %d\n",var_name,var_value);
            }
        }
    }
    fclose(file);
    return 0;
}