/*
 * program to write something on to the /dev/simple_device fle.
 * writing on to this file should invoke the driver to execute the read and write operation.
 */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	char s[100];
	int fd = open("/dev/simple_device",O_WRONLY);
	if(fd < 0)
	{
		perror("FILE:");
		return 0;
	}
	printf("Enter the data to write:");
	scanf("%s",s);
	write(fd,s,strlen(s)+1);
	close(fd);
        fd = open("/dev/simple_device",O_RDONLY);
	read(fd,s,strlen(s)+1);
	printf("received data= %s\n",s);
	close(fd);
	return 0;
}
