#include<stdio.h>
#include<conio.h>
#include<string.h>
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
        if(strncmp(line,"print",5)==0)
        {
            printf("%s",line+6);
        }
    }
    fclose(file);
    return 0;
}