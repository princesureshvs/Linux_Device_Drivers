#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>

int main()
{

    int fd = open("file",O_CREAT,0224);
    struct stat v;

    if(stat("file",&v)<0)
    {
	    perror("stat:\n");
	    return 0;
    }
    printf("%o\n",v.st_mode&0777);
    chmod("file",0664);
    
    if(stat("file",&v)<0)
    {
	    perror("stat:\n");
	    return 0;
    }
    printf("%o\n",v.st_mode&0777);
}
