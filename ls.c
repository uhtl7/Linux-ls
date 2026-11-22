#include<stdio.h>
#include<dirent.h>//处理文件和目录信息,DIR,
#include<sys/stat.h>//stat()
#include<stdlib.h>//getenv()
#include<string.h>
typedef struct{
    char d_name[256];
    struct stat st;
}Entry;
int compare_name(const void*a,const void*b)
{
    struct dirent* fp1=(struct dirent*)a;
    struct dirent* fp2=(struct dirent*)b;
    return strcmp(fp1->d_name,fp2->d_name);
}
int compare_time(const void*a,const void*b)
{
    Entry*n=a;
    Entry*m=b;
    if(n->st.st_mtime>m->st.st_mtime)
    return -1;
    if(n->st.st_mtime<m->st.st_mtime)
    return 1;
    return 0;
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
    struct dirent *readfile;
    Entry*entry;
    int n=0;
    int cap=0;
    char path[1024];
    while((readfile=readdir(dir))!=NULL){//遍历目录内容
        if(readfile->d_name[0]=='.')
           continue;
        if(n==cap){ 
        cap=cap?cap*2:64;
        entry=realloc(entry,cap*sizeof(Entry));
        }
        strncpy(entry[n].d_name,readfile->d_name,255);
        entry[n].d_name[255]='\0'; 
        snprintf(path,sizeof(path),"%s/%s",home,readfile->d_name);
        stat(path,&entry[n].st);
        n++;
    }
    closedir(dir);//关闭目录
    qsort(entry,n,sizeof(Entry),compare_time);
    for(int i=0;i<n;i++){
        printf("%s ",entry[i].d_name);
    }
    free(entry);
    printf("\n");
    return 0;
    // DIR *dir;
    // const char*home=getenv("HOME");
    // dir=opendir(home);
    // if(dir==NULL)
    // {
    //     perror("opendir");
    //     return 0;
    // }
    // struct dirent *readfile;
    // struct stat st;
    // while((readfile==readdir(dir))!=NULL)
    // {
    //     if(strcmp(readfile->d_name,'.')==0 || strcmp(readfile->d_name,'..')==0)
    //     continue;  
        
        
    // }
    // closedir(dir);
	// return 0;
}
