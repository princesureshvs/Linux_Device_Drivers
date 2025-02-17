/* Write a C program to log messages into a log file using both system calls and libc APIs.The program should allow the user to log messages to a file using two different methods: */

#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

/*
 * Here is a macro definition for printing, date,time,filename,function(main),log message, line number.
 * write(x,&a,sizeof(a)); This prints a in the binary form in file.when using cat we can see it as integer.
 * when the file is opened using vi it is seen as ^@@@.
 * solution: convert integer a to string using snprintf(buffer,sizeof(buffer),"%d",a);
 * Here the macro is used because we want the function to return file number where error occured.
 * Else it will go the function definition and that line number is returned.
 */

#define macro_sys_call(x,y)\
{                                            \
        int a = __LINE__;\
	char buffer[20];\
	write(x,__DATE__,strlen(__DATE__));\
	write(x, " ", 1);\
	write(x,__TIME__,strlen(__TIME__));\
	write(x, " ", 1);\
	write(x,__FILE__,strlen(__FILE__));\
	write(x, " ", 1);\
	write(x,__func__,strlen(__func__));\
	write(x, " ", 1);\
	write(x,s,strlen(y));\
	write(x, " ", 1);\
	snprintf(buffer, sizeof(buffer), "%d", a);\
	write(x,"\n",1);\
	close(x);\
}

/*
 * using gcc library functions for printing the data on to the file.
 */

#define macro_user_call(fp,s) \
{                                                                                             \
	fprintf(fp,"%s  %s  %s  %s  %d  %s\n",__DATE__,__TIME__,__FILE__,__func__,__LINE__,s);\
        fclose(fp);\
}

/*
 * Here from the user we are asking if he want to use lib api or system calls.
 * After that the log message is being scanned from the user.
 * Based on the user input it will call user api or s/m call.
 */

int main()
{
	int op;
	char s[50];
	printf("Methods to print into the log file:\n");
	printf("1.Lib API\n");
	printf("2.System calls\n");
	scanf("%d",&op);
	printf("Enter the log message: ");
	scanf(" %[^\n]",s);

	/*
	 * Case 1 opens logfile.txt and calls macro macro_user_call(fp,s).
	 * Case 2 opens the logfile.txt and calls macro_sys_call(fs,s).
	 * default it will return as invalid operation.
	 * provide 0664 while using O_CREAT.
	 */

	switch(op)
	{
		case 1:FILE *fp = fopen("logfile.txt","a");
		       if(fp < 0)
		       {
			       perror("FILE:");
			       return 0;
		       }
		       macro_user_call(fp,s);
		       printf("Log file saved successfully\n");
		       break;
		case 2:int fs = open("logfile.txt",O_WRONLY|O_APPEND|O_CREAT,0664);
		       if(fs < 0)
		       {
			       perror("FILE");
			       return 0;
		       }
		       macro_sys_call(fs,s);
		       printf("Log file saved successfully\n");
		       break;

		default:printf("Invalid option\n");break;
	}

}
