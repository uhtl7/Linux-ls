#include<stdio.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdlib.h>
int main()
{
    DIR *dir;
    struct dirent *readfile;
    dir=opendir(".");
    if(dir==NULL)
    {
        perror("Wrong");
        return 0;
    }
    while((readfile=readdir(dir))!=NULL)
    {
        if(readfile->d_name[0]=='.')
           continue;
        printf("%s ",readfile->d_name);   
    }
	return 0;
}
