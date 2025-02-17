#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
char result[20];
char s[20]="12";
char s1[20];
void doubled_fun()
{
	char *buff = s1;
	int value=0,rev=0;

	while( *buff != '\0')
	{
		if(*buff >= 48 && *buff <= 57)
		{
			value = value*10 + (*buff - 48);
		}
		buff++;
	}
	value = value * 2;
      /*  int i;
	for(i=0;value;value=value/10)
	{
		rev = rev*10 + (value%10);
	}
        value = rev;
        i=0;
	while(value)
	{
                s1[i]=(value%10)+48;
		value = value/10;
	}
	s[i]='\0';  
	*/

	sprintf(result,"%d",value);
	printf("value = %d\n",value);
	printf("result = %s\n",result);	
}

int main()
{
	int fp = open("file1.txt",O_RDONLY|O_WRONLY|O_CREAT,0664);
	write(fp,s,strlen(s)+1);
        close(fp);

	fp = open("file1.txt",O_RDONLY);
	read(fp,s1,strlen(s)+1);
	printf("data=%s\n",s1);
	close(fp);
	doubled_fun();
	
	int fd = open("file1.txt",O_WRONLY);
	write(fd,result,strlen(result)+1);
        close(fd);
}
