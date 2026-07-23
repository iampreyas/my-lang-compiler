#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
typedef struct
{
    char name[50];
    int value;
    char str_value[100];
    int is_string;
    int array_values[50];
    int is_array;
    int array_size;
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
            printf("[SET] %s=%d\n",name,val);
            return;
        }
    }
    strcpy(symbol_table[variable_count].name,name);
    symbol_table[variable_count].value=val;
    symbol_table[variable_count].is_string=0;
    printf("[SET] %s=%d\n",name,val);
    variable_count++;
}
void set_array_variable(char *name,int *values,int size)
{
    for(int i=0;i<variable_count;i++)
    {
        if(strcmp(symbol_table[i].name,name)==0)
        {
            symbol_table[i].is_array=1;
            symbol_table[i].array_size=size;
            for(int j=0;j<size;j++)
            {
                symbol_table[i].array_values[j]=values[j];
            }
            printf("[SET ARRAY] %s with %d elements\n",name,size);
            return;
        }
    }
    strcpy(symbol_table[variable_count].name,name);
    symbol_table[variable_count].is_array=1;
    symbol_table[variable_count].array_size=size;
    for(int j=0;j<size;j++)
    {
        symbol_table[variable_count].array_values[j]=values[j];
    }
    printf("[SET ARRAY] %s with %d elements\n",name,size);
    variable_count++;
}
void set_string_variable(char *name,char *str_val)
{
    for(int i=0;i<variable_count;i++)
    {
        if(strcmp(symbol_table[i].name,name)==0)
        {
            strcpy(symbol_table[i].str_value,str_val);
            symbol_table[i].is_string=1;
            printf("[SET] %s=\"%s\"\n",name,str_val);
            return;
        }
    }
    strcpy(symbol_table[variable_count].name,name);
    strcpy(symbol_table[variable_count].str_value,str_val);
    symbol_table[variable_count].is_string=1;
    printf("[SET] %s=\"%s\"\n",name,str_val);
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
    int run_block=1;
    int line_num=0;
    long loop_start_pos=0;
    int in_loop=0;
    long pos=ftell(file);
    int loop_condition_true=1;
    while(fgets(line,sizeof(line),file)!=NULL)
    {
        long current_line_start=pos;
        pos=ftell(file);
        line_num++;
        line[strcspn(line, "\r\n")]=0;
        if(execute_next_line==0 && strncmp(line,"else",4)!=0 && strncmp(line,"endif",5)!=0 && strncmp(line,"}",1)!=0 && strncmp(line,"endwhile",8)!=0)
        {
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
        printf("LINE:[%s]\n",line);
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
        else if(strncmp(line,"while",5)==0)
        {
            if(!in_loop)
            {
                loop_start_pos=current_line_start;
                in_loop=1;
            }
            char arg1[50]={0},arg2[50]={0},comp_op=0;
            if(sscanf(line+6,"%49[^<>=]%c%49s",arg1,&comp_op,arg2)==3)
            {
                printf("arg1=[%s] arg2=[%s]\n",arg1,arg2);
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
                printf("While check: %d %c %d\n",val,comp_op,val2);
                loop_condition_true=0;
                if(comp_op=='>'&& val>val2)
                {
                    loop_condition_true=1;
                }
                else if(comp_op=='<'&& val<val2)
                {
                    loop_condition_true=1;
                }
                else if(comp_op=='='&&val==val2)
                {
                    loop_condition_true=1;
                }
                else
                {
                    loop_condition_true=0;
                    execute_next_line=0;
                }
                printf("loop_condition_true=%d\n",loop_condition_true);
            }
            continue;
        }
        else if(strcmp(line,"endwhile")==0||strcmp(line,"end_while")==0)
        {
            printf("Endwhile: loop_condition_true=%d\n",loop_condition_true);
            if(loop_condition_true)
            {
                printf("Jumping to %ld\n",loop_start_pos);
                fseek(file,loop_start_pos,SEEK_SET);
                pos=loop_start_pos;
            }
            else
            {
                printf("Loop Finished\n");
                execute_next_line=1;
                in_loop=0;
                loop_condition_true=1;
            }
            continue;
        }
        else if(strcmp(line,"else")==0)
        {
            execute_next_line=!execute_next_line;
            continue;
        }
        else if(strcmp(line,"endif")==0||strcmp(line,"}")==0)
        {
            execute_next_line=1;
            continue;
        }
        else if(strncmp(line,"print",5)==0)
        {
            char format_string[100]={0};
            char var_name[50]={0};
            if(sscanf(line+6,"%[^,], %s",format_string,var_name)==2)
            {
                int is_variable=0;
                int val=get_variable(var_name,&is_variable);
                if(is_variable)
                {
                    int idx=-1;
                    for(int i=0;i<variable_count;i++)
                    {
                        if(strcmp(symbol_table[i].name,var_name)==0)
                        {
                            idx=i;
                        }
                    }
                    if(idx!=-1 && symbol_table[idx].is_string==1)
                    {
                        printf(format_string,symbol_table[idx].str_value);
                    }
                    else
                    {
                        printf(format_string,val);
                    }
                    printf("\n");
                    continue;
                }
            }
            else if(sscanf(line+6,"%[^,]%s",format_string,var_name)==2)
            {
                int is_variable=0;
                int val=get_variable(var_name,&is_variable);
                if(is_variable)
                {
                    int idx=-1;
                    for(int i=0;i<variable_count;i++)
                    {
                        if(strcmp(symbol_table[i].name,var_name)==0)
                        {
                            idx=i;
                        }
                    }
                    if(idx!=-1 && symbol_table[idx].is_string==1)
                    {
                        printf(format_string,symbol_table[idx].str_value);
                    }
                    else
                    {
                        printf(format_string,val);
                    }
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
            char str_val[100]={0};
            char arg1[50]={0};
            char arg2[50]={0};
            char array_str[100]={0};
            char op=0;
            int var_value;
            if(sscanf(line,"%49[^=]=[%99[^]]]",var_name,array_str)==2)
            {
                char *trimmed_var=var_name;
                while(*trimmed_var==' ')
                {
                    trimmed_var++;
                }
                char *end=trimmed_var+strlen(trimmed_var)-1;
                while(end>trimmed_var && *end==' ')
                {
                    *end='\0';
                    end--;
                }
                int values[50];
                int count=0;
                char *token=strtok(array_str,",");
                while(token!=NULL && count<50)
                {
                    values[count++]=atoi(token);
                    token=strtok(NULL,",");
                }
                set_array_variable(trimmed_var,values,count);
                continue;
            }
            char source_arr[50]={0};
            int index=0;
            if(sscanf(line,"%49[^=]=%49[^[][%d]",var_name,source_arr,&index)==3)
            {
                char *trimmed_var=var_name;
                while(*trimmed_var==' ')
                {
                    trimmed_var++;
                }
                char *end=trimmed_var+strlen(trimmed_var)-1;
                while(end>trimmed_var && *end==' ')
                {
                    *end='\0';
                    end--;
                }
                char *trimmed_arr=source_arr;
                while(*trimmed_arr==' ')
                {
                    trimmed_arr++;
                }
                end=trimmed_arr+strlen(trimmed_arr)-1;
                while(end>trimmed_arr && *end==' ')
                {
                    *end='\0';
                    end--;
                }
                for(int i=0;i<variable_count;i++)
                {
                    if(strcmp(symbol_table[i].name,trimmed_arr)==0 && symbol_table[i].is_array)
                    {
                        if(index>=0 && index<symbol_table[i].array_size)
                        {
                            set_variable(trimmed_var,symbol_table[i].array_values[index]);
                        }
                        break;
                    }
                }
                continue;
            }
            int update_idx=0,update_val=0;
            if(sscanf(line,"%49[a-zA-Z0-9_][%d]=%d",var_name,&update_idx,&update_val)==3)
            {
                for(int i=0;i<variable_count;i++)
                {
                    if(strcmp(symbol_table[i].name,var_name)==0 && symbol_table[i].is_array)
                    {
                        if(update_idx>=0 && update_idx<symbol_table[i].array_size)
                        {
                            symbol_table[i].array_values[update_idx]=update_val;
                            printf("[UPDATE ARRAY] %s[%d]=%d\n",var_name,update_idx,update_val);
                        }
                        break;
                    }
                }
                continue;
            }
            char len_arr_name[50]={0};
            if(sscanf(line," %49[a-zA-Z0-9_]=len(%49[a-zA-z0-9_])",var_name,len_arr_name)==2||sscanf(line,"%49[^=]=len(%49[^)])",var_name,len_arr_name)==2)
            {
                char *trimmed_var=var_name;
                while(*trimmed_var==' ')
                {
                    trimmed_var++;
                }
                char *end=trimmed_var+strlen(trimmed_var)-1;
                while(end>trimmed_var && *end==' ')
                {
                    *end='\0';
                    end--;
                }
                char *trimmed_arr=len_arr_name;
                while(*trimmed_arr==' ')
                {
                    trimmed_arr++;
                }
                end=trimmed_arr+strlen(trimmed_arr)-1;
                while(end>trimmed_arr && *end==' ')
                {
                    *end='\0';
                    end--;
                }
                for(int i=0;i<variable_count;i++)
                {
                    if(strcmp(symbol_table[i].name,trimmed_arr)==0 && symbol_table[i].is_array)
                    {
                        set_variable(trimmed_var,symbol_table[i].array_size);
                        printf("[ARRAY LEN] %s=%d\n",trimmed_var,symbol_table[i].array_size);
                        break;
                    }
                }
                continue;
            }
            if(sscanf(line,"%49[^=]=\"%99[^\"]\"",var_name,str_val)==2)
            {
                char *trimmed_var=var_name;
                while(*trimmed_var==' ')
                {
                    trimmed_var++;
                }
                char *end=trimmed_var+strlen(trimmed_var)-1;
                while(end>trimmed_var && *end==' ')
                {
                    *end='\0';
                    end--;
                }
                set_string_variable(trimmed_var,str_val);
                continue;
            }
            if(sscanf(line,"%49[^=]=%[^+*-/%^]%c%s",var_name,arg1,&op,arg2)==4)
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
                else if(op=='*')
                {
                    final_val=val1*val2;
                }
                else if(op=='/')
                {
                    if(val2!=0)
                    {
                        final_val=val1/val2;
                    }
                    else
                    {
                        printf("[ERROR on line %d] Division by Zero\n");
                        final_val=0;
                    }
                }
                else if(op=='%')
                {
                    if(val2!=0)
                    {
                        final_val=val1%val2;
                    }
                    else
                    {
                        printf("[ERROR on line %d] modulo by zero\n");
                        final_val=0;
                    }
                }
                else if(op=='^')
                {
                    final_val=(int)pow(val1,val2);
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