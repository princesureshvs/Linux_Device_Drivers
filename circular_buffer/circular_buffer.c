#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define PATH "/dev/logger_device"

void send_data(void);
void receive_data(void);

int main()
{
	int op;
	printf("Enter the option:\n");
	printf("1.Write to log buffer\n");
	printf("2.Read from the log buffer\n");
	printf("3.To display the whole buffer\n");
	printf("4.Exit\n");
	scanf("%d",&op);
	switch(op)
	{
		case 1:send_data();
		       break;
		case 2:receive_data();
		       break;
		//case 3:;
		  //     break;
		case 4:exit(0);
		default:printf("Invalid operation\n");
	}
	return 0;
}
void send_data()
{
	char s[100];
	int fd =  open(PATH,O_WRONLY);
	perror("fd:");
	printf("Enter the message:");
	scanf(" %[^\n]",s);
	write(fd,s,strlen(s)+1);
}
void receive_data()
{
	char s1[1024];
	int fd =  open(PATH,O_RDONLY);
	perror("fd:");
	read(fd,s1,sizeof(s1));
	printf("received data = %s\n",s1);
}
