/*
 * program to implement basic calculator, where the the expression is passed from the user space and the operations are done by the kernel device driver.
 */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	/*
	 * Opening the file descriptor on read only mode.
	 * Scanning the expression (eg: 2+2) from the user as a string into s buffer.
	 */
	char s[100],s1[100];
	int fd = open("/dev/simple_device",O_WRONLY);
	if(fd < 0)
	{
		perror("FILE:");
		return 0;
	}
	printf("Basic calculator function:\n");
	printf("Eg: 2+2\n");
	printf("Available operations + - /* /\n");
	printf("Enter the expression:");
	scanf("%[^\n]",s);
	/*
	 * Got the problem of displaying special characters  when i passed strlen(s)+1 to kernel using write().
	 * The problem was solved when i removed +1.
	 * While using read() try to put sizeof(s) rather than strlen, cause how do we know strlen of receiving string.
	 * Writing the data s buffer data to the /dev/simple_device.
	 */
	write(fd,s,strlen(s));
	close(fd);

	/*
	 * Opening the /dev/simple_device buffer and reading the buffer /dev/simple_device.
	 * This is to the read the processed data which have been saved in the kernel buffer.
	 * This buffer will be invoked if we read do read operation on the /dev/simple_device directory.
	 * That means if we do read operation on /dev/simple_driver it will invoke read function of driver code.
	 */

	fd = open("/dev/simple_device",O_RDONLY);
	read(fd,s1,sizeof(s1));
	printf("Answer= %s\n",s1);
	close(fd);
	return 0;
}
