#include<stdio.h>
#include<dirent.h>//处理文件和目录信息,DIR,
#include<sys/stat.h>//stat()
#include<stdlib.h>//getenv()
#include<string.h>//strcoll()
#include<unistd.h>//getopt()
#include<pwd.h>//getpwuid()
#include<grp.h>//getpegid()
#include<time.h>
#include<locale.h>//LC_COLLATE
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
const char *get_color(struct stat *st)
{
   if(S_ISDIR(st->st_mode))
   return "\033[1;34m";//蓝色(加粗)，目录
   if(S_ISLNK(st->st_mode))
   return "\033[1;36m";//青色(加粗)，连接
   if((S_ISREG(st->st_mode) && (st->st_mode & 0111)))
   return "\033[1;32m";//绿色(加粗)，可执行文件
   return "\033[0m";//默认颜色
}
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
    const Entry*n=a;
    const Entry*m=b;
    if(n->st.st_mtime>m->st.st_mtime)
    return -1;
    if(n->st.st_mtime<m->st.st_mtime)
    return 1;
    return 0;
}
int compare_name(const void*a,const void*b)
{
    const Entry*n=a;
    const Entry*m=b;
    return strcoll(n->file_name,m->file_name);
}
void get_permission(mode_t mode,char perm[11])
{
    perm[0]=S_ISDIR(mode) ? 'd' : S_ISLNK(mode) ? 'l' : '-';
    perm[1]=(mode & S_IRUSR) ? 'r' : '-';
    perm[2]=(mode & S_IWUSR) ? 'w' : '-';
    perm[3]=(mode & S_IXUSR) ? 'x' : '-';
    perm[4]=(mode & S_IRGRP) ? 'r' : '-';
    perm[5]=(mode & S_IWGRP) ? 'w' : '-';
    perm[6]=(mode & S_IXGRP) ? 'x' : '-';
    perm[7]=(mode & S_IROTH) ? 'r' : '-';
    perm[8]=(mode & S_IWOTH) ? 'w' : '-';
    perm[9]=(mode & S_IXOTH) ? 'x' : '-';
    perm[10]='\0';
}
char *get_uid(char uid_str[],int sz,uid_t uid)
{
    struct passwd*pw=getpwuid(uid);
    snprintf(uid_str,sz,"%s",pw->pw_name);
    return uid_str;
}
char *get_gid(char gid_str[],int sz,gid_t gid)
{
    struct group*gr=getgrgid(gid);
    snprintf(gid_str,sz,"%s",gr->gr_name);
    return gid_str;
}
char *get_time(char time_str[],int sz,time_t mtime)
{
    struct tm*tm=localtime(&mtime);
    strftime(time_str,sz,"%m月 %d %2H:%M",tm);
    return time_str;
}
void print(Entry*entry,int n,Input *input)
{
    if(n==0)
    return;
    if(input->l)
        {
            char perm[20];
            char uid_str[20];
            char gid_str[20];
            char time_str[100];
            for(int i=0;i<n;i++)
            {
                if(input->i)
                printf("%d ",(int)entry[i].st.st_ino);
                if(input->s)
                printf("%d ",(int)entry[i].st.st_blocks);
                get_permission(entry[i].st.st_mode,perm);
                char*user=get_uid(uid_str,sizeof(uid_str),entry[i].st.st_uid);
                char*group=get_gid(gid_str,sizeof(gid_str),entry[i].st.st_gid);
                char*time=get_time(time_str,sizeof(time_str),entry[i].st.st_mtime);
                printf("%-10s%2ld %-8s%-8s%6ld %s ",perm,entry[i].st.st_nlink,user,group,entry[i].st.st_size,time);
                printf("%s%s%s\n",get_color(&entry[i].st),entry[i].file_name,"\033[0m");
            }
            return; 
        }
    int max_len=0;
    for(int i=0;i<n;i++)
    {
        int len=strlen(entry[i].file_name);
        if(len>max_len)
        max_len=len;
    }
    int width=80;
    int col_width=max_len+2;
    int col=width/col_width;
    if(col<1)
    col=1;
    if(col>n)
    col=n;
    int row=(n+col-1)/col;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            int m=j*row+i;
            if(m>=n)
            continue;
            if(input->i)
            printf("%d ",(int)entry[m].st.st_ino);
            if(input->s)
            printf("%d ",(int)entry[m].st.st_blocks);
            printf("%s%-*s%s",get_color(&entry[m].st),col_width,entry[m].file_name,"\033[0m");
        }
        printf("\n");
    }
}
void list_file(char *path,Input *input)
{
    if(input->R)
    printf("%s:\n",path);
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
    // char perm[20];
    // char uid_str[20];
    // char gid_str[20];
    // char time_str[100];
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
        snprintf(fpath,sizeof(fpath),"%s/%s",path,dp->d_name);
        lstat(fpath,&entry[n].st);
        n++;
    }
    closedir(dir);
    if(input->t)
    qsort(entry,n,sizeof(entry[0]),compare_time);
    else
    qsort(entry,n,sizeof(entry[0]),compare_name);
    if(input->r)
    {
        for(int i=0;i<n;i++)
        {
            int left=i;
            int right=n-i-1;
            if(left<right)
            {
                Entry t=entry[left];
                entry[left]=entry[right];
                entry[right]=t;
            }
        }
    }
    // for(int i=0;i<n;i++)
    // {
    //     if(input->i)
    //     printf("%d ",(int)entry[i].st.st_ino);
    //     if(input->s)
    //     printf("%d ",(int)entry[i].st.st_blocks);
    //     if(input->l)
    //     {
    //         get_permission(entry[i].st.st_mode,perm);
    //         char*user=get_uid(uid_str,sizeof(uid_str),entry[i].st.st_uid);
    //         char*group=get_gid(gid_str,sizeof(gid_str),entry[i].st.st_gid);
    //         char*time=get_time(time_str,sizeof(time_str),entry[i].st.st_mtime);
    //         printf("%s %ld %s %s %ld %s ",perm,entry[i].st.st_nlink,user,group,entry[i].st.st_size,time);
    //     }
    // }
    print(entry,n,input);
    if(input->R)
    {
       for(int i=0;i<n;i++)
       {
        if(strcmp(entry[i].file_name,".")==0 || strcmp(entry[i].file_name,"..")==0)
        continue;
        if(!S_ISDIR(entry[i].st.st_mode))//判断是否为目录
        continue;
        char new_path[4096];
        snprintf(new_path,sizeof(new_path),"%s/%s",path,entry[i].file_name);
        list_file(new_path,input);
       }  
    }
    free(entry);
}
int main(int argc,char*argv[])
{
   setlocale(LC_COLLATE, "");
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