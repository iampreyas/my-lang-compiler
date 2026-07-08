#include<stdio.h>
#include<conio.h>
int main()
{
    FILE *file=fopen("./examples/hello.cris","r");
    if(file==NULL)
    {
        printf("Error\n");
        return 1;
    }
    printf("hello.cris opened successfully!\n");
    char line[100];
    while(fgets(line,sizeof(line),file)!=NULL)
    {
        printf("%s",line);
    }
    fclose(file);
    return 0;
}