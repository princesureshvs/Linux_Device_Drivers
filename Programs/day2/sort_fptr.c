/*
 * OBJECTIVE: Implement a sorting function that takes an array of integers and a comparison function pointer to allow sorting in ascending or descending.
 */

#include<stdio.h>
typedef void (*fptr)(int *,int );// fptr is a fn pointer pointing to a function which takes 2 arguments(int *,int) and returns void as argument.                                                             fptr is the new datatype now 
void sort_fn(int *a,int ele,fptr fn)//This acts as a callback fn which takes 3 arguments one of which is the fptr which takes the function name.
{
      fn(a,ele); // The fn name which is passed down is invoked here at place of fn
}

void ascending(int *a,int ele); 
void descending(int *a,int ele);

int main()
{
	int a[6],ele,i,op;
	printf("Enter the elements:\n");
        ele=sizeof(a)/sizeof(a[0]);
	/* scanning the elements. */
	for(i=0;i<ele;i++)
		scanf("%d",&a[i]);

	/*
	 * Here the option scanning takes place.
	 * Based on the input different callback functions are called. 
	 */
	printf("select the sorting method:\n");
	printf("1.Ascending\n");
	printf("2.Descending\n");
	scanf("%d",&op);
	if(op==1)
	{
		sort_fn(a,ele,ascending); //This is a callback fn
	}
	else if(op==2)
	{
		sort_fn(a,ele,descending);
	}
	else
	{
		printf("Wrong option\n");
		return 0;
	}

	printf("After Sorting:\n");
	for(i=0;i<ele;i++)
		printf("%d ",a[i]);
	printf("\n");
	return 0;
}

/*
 * This is where the functions for ascending and descending sorting are written
 */

void ascending(int *a,int ele)
{
       int i,j,t;
       for(i=0;i<ele;i++)
       {
	       for(j=i+1;j<ele;j++)
	       {
		       if(a[i]>a[j])
		       {
			       t=a[i];
			       a[i]=a[j];
			       a[j]=t;
		       }
	       }
       } 
}
void descending(int *a,int ele)
{
	int i,j,t;
	for(i=0;i<ele;i++)
	{
		for(j=i+1;j<ele;j++)
		{
			if(a[i]<a[j])
			{
				t=a[i];
				a[i]=a[j];
				a[j]=t;
			}
		}
	} 
}
