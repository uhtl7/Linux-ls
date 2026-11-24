#include<stdio.h>
#include<dirent.h>//处理文件和目录信息,DIR,
#include<sys/stat.h>//stat()
#include<stdlib.h>//getenv()
#include<string.h>
#include<unistd.h>//getopt()
#include<pwd.h>//getpwuid
#include<grp.h>//getpegid
#include<time.h>
typedef struct{
    char file_name[256];
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
    return optind;
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
int compare_name(const void*a,const void*b)
{
    struct dirent* fp1=(struct dirent*)a;
    struct dirent* fp2=(struct dirent*)b;
    return strcmp(fp1->d_name,fp2->d_name);
}
// void get_file(char *path,struct stat *st)
// {
//     lstat(path,st);
// }
void get_permission(mode_t mode,char perm[11])
{
    perm[0]=S_ISDIR(mode) ? 'd' : S_ISLNK(mode) ? 'l' : '-';
    perm[1]=(mode & S_IRUSR) ? 'r' : '-';
    perm[2]=(mode & S_IWUSR) ? 'w' : '-';
    perm[3]=(mode & S_IXUSR) ? 'x' : '-';
    perm[4]=(mode & S_IRUSR) ? 'r' : '-';
    perm[5]=(mode & S_IWUSR) ? 'w' : '-';
    perm[6]=(mode & S_IXUSR) ? 'x' : '-';
    perm[7]=(mode & S_IRUSR) ? 'r' : '-';
    perm[8]=(mode & S_IWUSR) ? 'w' : '-';
    perm[9]=(mode & S_IXUSR) ? 'x' : '-';
    perm[10]='\0';
}
char *get_uid(uid_t uid)
{
    struct passwd*pw=getpwuid(uid);
    char uid_str[20];
    snprintf(uid_str,sizeof(uid_str),"%u",uid);
    return uid_str;
}
char *get_gid(gid_t gid)
{
    struct group*gr=getgrgid(gid);
    char gid_str[20];
    snprintf(gid_str,sizeof(gid_str),"%u",gid);
    return gid_str;
}
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
    char perm[20];
    int n=0;
    int cap=0;
    Entry*entry=NULL;
    while((dp=readdir(dir))!=NULL)
    {
        if(input->a==0 && dp->d_name[0]=='.')
           continue;
        if(n==cap)
        {
            cap=cap?cap*2:128;
            entry=realloc(entry,sizeof(entry[0])*cap);
        }
        snprintf(entry[n].file_name,sizeof(entry[n].file_name),"%s",dp->d_name);   
        snprintf(fpath,sizeof(fpath),"%s/%s", path, dp->d_name);
        lstat(fpath,&entry[n].st);
        n++;
    }
    closedir(dir);
    if(input->t)
    qsort(entry,n,sizeof(entry[0]),compare_time);
    else
    qsort(entry,n,sizeof(entry[0]),compare_name);
    for(int i=0;i<n;i++)
    {
        int left=i;
        int right=n-i-1;
        if(input->r && left<right)
        {
            Entry t=entry[left];
            entry[left]=entry[right];
            entry[right]=t;
        }
        if(input->i)
        printf("%d ",(int)entry[i].st.st_ino);
        if(input->s)
        printf("%d ",(int)entry[i].st.st_blocks);
        if(input->l)
        {
            get_permission(entry[i].st.st_mode,perm);
            char*user=get_uid(entry[i].st.st_uid);
            char*group=get_gid(entry[i].st.st_gid);
            printf("%s %ld %s %s %ld ",perm,entry[i].st.st_nlink,user,group,entry[i].st.st_size);
        }
        printf("%-*s",strlen(entry[i].file_name)+2,entry[i].file_name);
    }
    free(entry);
    printf("\n");
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
//-l  -R
