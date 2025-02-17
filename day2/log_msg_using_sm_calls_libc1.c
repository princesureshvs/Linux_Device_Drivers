#include<stdio.h>
#include<string.h>
#include<fcntl.h>
void lib_log(char *s);
void sys_call(char *s);

char s[30];

int main()
{
	int op;
	printf("Methods to print into the log file:\n");
	printf("1.Lib Calls\n");
	printf("2.System calls\n");
	scanf("%d",&op);
	if(op==1)
	{
		printf("Enter the log message: ");
		scanf(" %[^\n]",&s);
		lib_log(s);
	}
	else if(op ==2 )
	{
             
		printf("Enter the log message: ");
		scanf(" %[^\n]",&s);
		sys_call(s);
	}
	else
	{
		printf("Invalid operation selected\n");
	        return 0;
	}
}

void lib_log(char *s)
{

	FILE *fp = fopen("log.txt","w+");
	fprintf(fp,"%s  %s  %s  %s  %d  %s\n",__DATE__,__TIME__,__FILE__,__func__,__LINE__,s);
	fclose(fp);
}

void sys_call(char *s)
{
      int fd = open("log.txt",O_CREAT|O_RDWR|O_APPEND,0664);
      if(fd < 0)
      {
	      perror("open");
      }

      write(fd,__DATE__,strlen(__DATE__));
      write(fd,__TIME__,strlen(__TIME__));
      write(fd,__FILE__,strlen(__FILE__));
      write(fd,__func__,strlen(__func__));
      //write(fd,__LINE__,strlen(__LINE__));
      write(fd,s,strlen(s));

      close(fd);
}

