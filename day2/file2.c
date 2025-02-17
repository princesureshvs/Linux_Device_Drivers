#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
int main()
{
	char s[20]="prince";

	write(1,s,strlen(s)+1);
	printf("\n");
}
