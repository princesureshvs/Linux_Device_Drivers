/*
 * OBJECTIVE: Write a program that reads an array of integers and applies a callback function to each element to transform the values (e.g., doubling, squar   ing).
 */


#include<stdio.h>

/*
 * function pointer datatype creation takes place.
 * callback fn declaration and definition is present here.
 * doubling and square fn declaration.
 */

typedef void (*fptr)(int *);
void callback_fn(int *a,fptr fn)
{
	fn(a);
}

void doubling(int *);
void square(int *); 

int main()
{
	int a[7],i,ele,op;
	ele = sizeof(a)/sizeof(a[0]);
	printf("Enter the elements:\n");
	for(i=0;i<ele;i++)
		scanf("%d",&a[i]);

	printf("Select operation to do:\n");
	printf("1.Doubling\n");
	printf("2.Square\n");
	scanf("%d",&op);
	if(op==1)
	{
		// Here in the loop each each time iam passing an element of an array to the callback function.
		for(i=0;i<ele;i++)
		{
			callback_fn(&a[i],doubling);
		}
	}
	else if(op==2)
	{ 
		// Here in the loop each each time iam passing an element of an array to the callback function.
		for(i=0;i<ele;i++)
		{
			callback_fn(&a[i],square);
		}
	}
	else
	{
		printf("Wrong option:\n");
		return 0;
	}
	printf("output:\n");

        // Array is printed as the output.
	for(i=0;i<ele;i++)
		printf("%d ",a[i]);
	printf("\n");
	return 0;

}

/*
 * definition of doubling and square fn.
 */
void doubling(int *p)
{
	*p=*p * 2; 
}
void square(int *p)
{
	*p=(*p)*(*p);
}	
