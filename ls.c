#include<stdio.h>
#include<dirent.h>//处理文件和目录信息
#include<stdlib.h>
#include<string.h>
int compare(const void*a,const void*b)
{
    return (*(char*)a-*(char*)b);
}
int main()
{
    DIR *dir;
    const char*home=getenv("HOME");
    dir=opendir(home);//打开根目录
    if(dir==NULL)
    {
        perror("opendir");
        return 0;
    }
    char **name=NULL;
    int n=0;
    int cap=0;
    struct dirent *readfile;
    while((readfile=readdir(dir))!=NULL)//遍历目录内容
    {
        if(readfile->d_name[0]=='.')
           continue;
        if(n==cap){ 
        cap=cap?cap*2:64;
        name=realloc(name,cap*sizeof(char*));
        } 
        name[n++]=strdup(readfile->d_name);
    }
    closedir(dir);//关闭目录
    qsort(name,n,sizeof(char*),compare);
    for(int i=0;i<n;i++){
        printf("%s ",name[i]);
        free(name[i]);
    }
    free(name);
    printf("\n");
	return 0;
}
