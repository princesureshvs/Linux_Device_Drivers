#include<stdio.h>
int add(int a,int b)
{
	return a+b;
}
int multiply(int a, int b)
{
	return a*b;
}
int substract(int a,int b)
{
	return a-b;
}
typedef int (*fptr)(int, int);
int callback(int a, int b, fptr operation) 
{ 
	return operation(a, b);
}
int main()
{
	int a,b,result=0;
	int op;
	printf("Enter the two numbers a and b\n");
	scanf("%d %d",&a,&b);
	printf("Enter the operation to be done:\n");
	printf("1) addition\n2)multiplication\n3)substraction\n");
	scanf(" %d",&op);

	switch(op)
	{
		case 1: result = callback(a,b,add);break;
		case 2:	result = callback(a,b,multiply);break;
		case 3: result = callback(a,b,substract);break;
		default :break;
	}
	printf("result = %d\n",result);
}
