#include<stdio.h>
#include<dirent.h>//处理文件和目录信息,DIR,
#include<sys/stat.h>//stat()
#include<stdlib.h>//getenv()
#include<string.h>
int compare_name(const void*a,const void*b)
{
    struct dirent* fp1=(struct dirent*)a;
    struct dirent* fp2=(struct dirent*)b;
    return strcmp(fp1->d_name,fp2->d_name);
}
//     DIR *dir;
//     const char*home=getenv("HOME");
//     dir=opendir(home);//打开根目录
//     if(dir==NULL)
//     {
//         perror("opendir");
//         return 0;
//     }
//     char **name=NULL;
//     int n=0;
//     int cap=0;
//     struct dirent *readfile;
//     while((readfile=readdir(dir))!=NULL){//遍历目录内容
//         if(readfile->d_name[0]=='.')
//            continue;
//         if(n==cap){ 
//         cap=cap?cap*2:64;
//         name=realloc(name,cap*sizeof(char*));
//         } 
//         name[n++]=strdup(readfile->d_name);
//     }
//     closedir(dir);//关闭目录
//     qsort(name,n,sizeof(char*),compare_name);
//     for(int i=0;i<n;i++){
//         printf("%s ",name[i]);
//         free(name[i]);
//     }
//     free(name);
//     printf("\n");
int main()
{
    DIR *dir;
    const char*home=getenv("HOME");
    dir=opendir(home);
    if(dir==NULL)0000
    {
        perror("opendir");
        return 0;
    }
    struct dirent *readfile;
    struct stat st;
    while((readfile==readdir(dir))!=NULL)
    {
        if(strcmp(readfile->d_name,'.')==0 || strcmp(readfile->d_name,'..')==0)
        continue;
        
    }
    closedir(dir);
	return 0;
}
