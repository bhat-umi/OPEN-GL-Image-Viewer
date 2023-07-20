#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_PATH 100
int list(int count,const char *path,char *files[],const char *type);
int list_image(const char *path,char *files[]);

int list_image(const char *path,char *files[])
{
    int count=0;
    count=list(count,path,files,"/*.jpeg");
    count=list(count,path,files,"/*.jpg");
    count=list(count,path,files,"/*.png");
    return count;
}
 int list(int count,const char *path,char *files[],const char *type)
 {
    char temp[MAX_PATH]="ls ";
    strcat(temp,path);
    strcat(temp,type);
    FILE *p=popen(temp,"r");
    char item[MAX_PATH];
    while(fgets(item,MAX_PATH,p)!=NULL)
    {
        //checking response
        char temp_item[MAX_PATH];
        char *i=(char *)calloc(MAX_PATH,sizeof(char));
        sprintf(temp_item,"%s",item);
        //removing extra white space or \n
        strncpy(i,temp_item,(int)strlen(temp_item)-1);
      
        files[count++]=i;
        //printf("(%s) \n ",files[count-1]);
    }
    pclose(p);
    return count;


 }

