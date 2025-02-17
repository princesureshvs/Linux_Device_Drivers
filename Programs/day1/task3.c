#include<stdio.h>
int main()
{
	FILE *fp = fopen("temp.txt","w+");
	if(fp == 0)
	{
		perror("fp");
	}
	char s1[20]="hello",s2[20];
	fprintf(fp,"%s",s1);
	fclose(fp);

	fp=fopen("temp.txt","r");
	if(fp == 0)
	{
		perror("fp");
	}

	fscanf(fp,"%s",s2);
	printf("Output: %s\n",s2);
	fclose(fp);
}
