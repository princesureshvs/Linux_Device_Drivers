#include<stdio.h>
int sum(int a,int b)
{
	return a+b;
}

int mul(int a,int b)
{
	return a*b;
}

struct st
{
	int(*fptr)(int a,int b); //function pointer acts as a member and not a function here(acts as a pointer )
};

int main()
{
	int result,op;
	int m=10,n=20;
	struct st v;    // creating an instance of the structure
	
	// The basic working 
	//v.fptr = sum;
	//result = v.fptr(10,20);
	//printf("Enter the operands:\n");

	printf("Enter what operation to do:\n");
	printf("1.+\n2.*\n");
	scanf("%d",&op);

	switch(op)
	{
		case 1:v.fptr=sum; //assigning the fptr through the structure variable v and assigning sum function address             
		       result = v.fptr(m,n);// this acts as sum(m,n)   
		       break;
		case 2:v.fptr=mul;
		       result = v.fptr(m,n);
		       break;
		default:break;
	}
	printf("%d\n",result); //printing the result stored 
	return 0;
}
