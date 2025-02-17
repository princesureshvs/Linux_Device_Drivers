/* problem: Key-Value Store Using Device Driver
   Create a character device driver named kv_store.
   Allow the user-space application to:
   Write a key-value pair in the format key,value using write().
   Read the value corresponding to a key using read().
   Store key-value pairs in the kernel module using an appropriate data structure (e.g., a hash table).
   Support up to 10 key-value pairs. Return an error if the limit is exceeded.
   */


#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define PATH "/dev/kv_logger"

void add_keyvalue(void);
void retrive_keyvalue(void);

char s[20];

int main()
{
      int op;
      static int count=0;
      while(1)
       {
        /* choosing the option from the user to add the key value or return the value related to key */
         printf("Enter the option:\n");
         printf("1.To input the key and its value:\n");
         printf("2.To retrieve the value related to key:\n");
         printf("3.Exit:\n ");
         scanf("%d",&op);
         printf("\n");

	 switch(op)
	 {
		 case 1: if(count >= 5)
			{
                                printf("Structure limit exceeded\n");
				printf("\n");
				break;
                        }
			 add_keyvalue();count++;
			 break;
		 case 2:retrive_keyvalue();break;
		 case 3:exit(0);break;
		 default:printf("Invalid option\n");break;
	 }
       }
      return 0;
}

/* Function for adding the key and  value */
void add_keyvalue(void)
{
	/* scanning the key and value in the format key value from the user */
	printf("Enter the key and value:\n");
	printf("Format: key space value\n");
	scanf(" %[^\n]",s);

	/* Opening the file in write only format to write the key and its corresponding value */
	int fd = open(PATH,O_WRONLY);
	if(fd < 0)
	{
		perror("FILE");
		return ;
	}
	/* Doing the write operation */
        write(fd,s,strlen(s)+1);
        /* closing the file descriptor after writing */
        close(fd);
}

/* Function for reading the key and value from the kernel buffer */
void retrive_keyvalue(void)
{
	char s1[20];
	printf("Enter the key: ");
	scanf("%s",s1);
	/* sending the key value through read function buffer itself.
	 * printing the received key and value on to the screen.
	 */
	int fd = open(PATH,O_RDONLY);
	read(fd,s1,sizeof(s1));
	printf("The value = %s\n",s1);
	printf("\n");
	close(fd);
}


