#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

char s[20];

void basic_calculator()
{
	int x=0,y=0,result=0,flag=0,i=0;
        char op;

	while(s[i] != '\0')
	{
		if(s[i] >= '0' && s[i] <= '9')
		{
			if(flag == 0)
			{
				x = (x*10) + (s[i]-48);
				printf("x=%d\n",x);
			}
			if(flag == 1)
			{
				y = (y*10) + (s[i]-48);
				printf("y=%d\n",y);
			}
		}
		else
		{
			op = s[i];
			flag = 1;
		}
		i++;
	}

	printf("x=%d\n",x);
	printf("op=%c\n",op);
	switch(op)
	{
            case '+': result = x+y;printf("result=%d\n",result);sprintf(s,"%d",result);break;
            case '-': result = x-y;printf("result=%d\n",result);sprintf(s,"%d",result);break;
            case '*': result = (x*y);printf("result=%d\n",result);sprintf(s,"%d",result);break;
            case '/': result = (x/y);printf("result=%d\n",result);sprintf(s,"%d",result);break;
	    default: break;
	}
}

int main()
{
  char s1[20];	
  printf("Enter the string: ");
  scanf("%s",s);

  int fd = open("test_file.txt",O_WRONLY | O_CREAT,0664);
  write(fd,s,strlen(s)+1);
  close(fd);

  fd = open("test_file.txt",O_RDONLY);
  read(fd,s,strlen(s)+1);
  close(fd);
  printf("read in main = %s\n",s);

  basic_calculator();

  printf("In main after op = %s\n",s);
  printf("In main after s = %s\n",s);

  /* Before writing the result just clear the buffer */
  fd = open("test_file.txt",O_WRONLY|O_TRUNC);
  write(fd,s,strlen(s)+1);
  close(fd);

  fd = open("test_file.txt",O_RDONLY);
  read(fd,s,sizeof(s));
  close(fd);
  printf("last read = %s\n",s);
}
