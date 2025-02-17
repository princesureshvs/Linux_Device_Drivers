/* Basic calcualtor function implementation */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "simple_device"
#define BUFFER_SIZE 1024

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;

// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);

// File operations structure
static struct file_operations fops = {
    .open = device_open,
    .release = device_close,
    .read = device_read,
    .write = device_write,
};

/*
 * Here i have used x and y variable for the operand 1 and operand 2. 
 * Result to store result and op for storing operation.
 * I have taken a pointer to the device_buffer where the received string is stored.
 * Used a while loop to iterate till i get a null character in the string.
 * flag = NULL initilly so it will execute flag == 0 part,take the ascii value and store as int value in the x variable.
 * If the operator is reached it will set flag == 1. Store op = that operator.
 * Now it executes if(flag==1) part and stores y variable.
 * After that executes the switch case.
 */


static void basic_calculator(void)
{
        int x=0,y=0,result=0,flag=0;
        char op;
	char *buff = device_buffer;

        while(*buff != '\0')
        {
                if(*buff >= '0' && *buff <= '9')
                {
                        if(flag == 0)
                        {
                                x = (x*10) + (*buff-48);
                        }
                        if(flag == 1)
                        {
                                y = (y*10) + (*buff-48);
                        }
                }
                else
                {
                        op = *buff;
                        flag = 1;
                }

                buff++;
        }
	/* used to clear device_buffer */
        memset(device_buffer,0,sizeof(device_buffer));
        switch(op)
        {
	    /* sprintf is used to convert integer to string and print it on to the device_buffer */
            case '+': result = x+y;sprintf(device_buffer,"%d",result);break;
            case '-': result = x-y;sprintf(device_buffer,"%d",result);break;
            case '*': result = (x*y);sprintf(device_buffer,"%d",result);break;
            case '/': result = (x/y);sprintf(device_buffer,"%d",result);break;
            default: break;
        }
}




// Called when the device is opened
static int device_open(struct inode *inode, struct file *file) {
    open_count++;
    printk(KERN_INFO "simple_device: Device opened %d time(s)\n", open_count);
    return 0;
}

// Called when the device is closed
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device: Device closed\n");
    return 0;
}

// Called when data is read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    size_t len = strlen(device_buffer);

    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;

    if (copy_to_user(user_buffer, device_buffer + *offset, size)) {
        return -EFAULT;
    }

    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
        size = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, user_buffer, size)) {
        return -EFAULT;
    }

    device_buffer[size] = '\0'; // Null-terminate the string
    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", size);

    /* calculator function is called */

    basic_calculator();
    return size;
}

// Module initialization
static int __init simple_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_device: Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "simple_device: Registered with major number %d\n", major_number);
    return 0;
}

// Module cleanup
static void __exit simple_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_device: Unregistered device\n");
}

module_init(simple_driver_init);
module_exit(simple_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple Linux Device Driver");
