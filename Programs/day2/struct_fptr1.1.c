/*
 * Implement a menu-driven program where each menu option is a separate function, and the menu is managed using function pointers.
 */


#include <stdio.h>

typedef int(*fptr)(int, int);

int sum(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    int result, op;
    int m = 10, n = 20;

    enum { SUM = 1, MUL };

    // Function pointer array
    fptr a[2] = {sum, multiply};

    while (1) {
	    printf("\nChoose an operation:\n");
	    printf("1. Sum\n");
	    printf("2. Multiply\n");
	    printf("3. Exit\n");
	    printf("Enter your choice: ");
	    scanf("%d", &op);

	    switch (op) {
		    case SUM:
			    result = a[SUM - 1](m, n);  // Call the sum function
			    printf("Sum = %d\n", result);
			    break;

		    case MUL:
			    result = a[MUL - 1](m, n);  // Call the multiply function
			    printf("Product = %d\n", result);
			    break;

		    case 3:
			    printf("Exited....\n");
			    return 0;

		    default:
			    printf("Wrong choice\n");
	    }
    }

    return 0;
}

