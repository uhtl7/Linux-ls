#include<stdio.h>
#include<dirent.h>//处理文件和目录信息,DIR,
#include<sys/stat.h>//stat()
#include<stdlib.h>//getenv()
#include<string.h>
#include<unistd.h>//getopt()
typedef struct{
    struct stat st;
}Entry;
typedef struct{
    int a;
    int l;
    int R;
    int r;
    int i;
    int t;
    int s;
}Input;
int get_input(int argc,char *const argv[],Input*input)
{
    int ch=0;
    while((ch=getopt(argc,argv,"alRrits"))!=-1)
    {
       switch(ch)
       {
            case 'a':
                input->a=1;
                break;
            case 'l':
                input->l=1;
                break;
            case 'R':
                input->R=1;
                break;
            case 'r':
                input->r=1;
                break;
            case 'i':
                input->i=1;
                break;
            case 't':
                input->t=1;
                break;
            case 's':
                input->s=1;
                break;
            //default     
       }
    }
    return 1;//?????;
}
// void get_file(char *path,struct stat *st)
// {
//     lstat(path,st);
// } 
void list_file(char *path,Input *input)
{
    DIR*dir;
    dir=opendir(path);
    if(dir==NULL)
    {
        perror("opendir");
        return;
    }
    struct dirent *dp;
    struct stat st;
    char fpath[4096];
    while((dp=readdir(dir))!=NULL)
    {
        if(input->a==0 && dp->d_name[0]=='.')
           continue;
        snprintf(fpath,sizeof(fpath),"%s/%s", path, dp->d_name);
        lstat(fpath,&st);
        if(input->i)
        printf("%d ",(int)st.st_ino);
        if(input->s)
        printf("%d ",(int)st.st_blocks);
        printf("%s\n",dp->d_name);
    }
}
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
int main(int argc,char*argv[])
{
   Input input={0};
   int file_path=get_input(argc,argv,&input);
   if(file_path==argc)
   {
        list_file(".",&input);
   }
   else
   {
        for(int i=file_path;i<argc;i++)
        {
            list_file(argv[i],&input);
        }
   }
   return 0;
}
