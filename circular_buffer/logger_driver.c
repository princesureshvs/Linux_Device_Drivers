#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "logger_device"
#define BUFFER_SIZE 1024
#define N 5

static void display(void);
static void enque(void);
static int deque(void);

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;

static struct que{
	char msg[1024];
}Q[5];

static int front = -1;
static int rear = -1;

static void enque(void)
{
	char *message = device_buffer;
	if(front==-1 && rear==-1)
	{
		front=rear=0;
		strcpy(Q[rear].msg,message);
		//strncpy(Q[rear].msg,message,sizeof(Q[rear].msg)-1);
		//Q[rear].msg[sizeof(Q[rear].msg)-1]='\0';
		printk(KERN_INFO "message inserted=%s\n",Q[rear].msg);
	}
	else if(((rear+1)%N)==front)
	{
		printk(KERN_INFO "que is full\n");
		return;
	}
	else
	{
		rear=(rear+1)%N;
		strcpy(Q[rear].msg,message);
		printk(KERN_INFO "message inserted=%s\n",Q[rear].msg);
		//strncpy(Q[rear].msg,message,sizeof(Q[rear].msg)-1);
		//Q[rear].msg[sizeof(Q[rear].msg)-1]='\0';
	}
}

static int deque(void)
{

	if(front==-1 && rear==-1)
	{
		printk(KERN_INFO "que is empty\n");
		return 0;
	}
	else if(front == rear)
	{
		front=rear=-1;
	}
	else
	{
		printk(KERN_INFO "The dequed element is = %s\n",Q[front].msg);	
		strcpy(device_buffer,Q[front].msg);
		//strncpy(Q[front].msg,message,sizeof(Q[front].msg)-1);
		//Q[front].msg[sizeof(Q[front].msg)-1]='\0';
		printk(KERN_INFO "device buffer in deque  = %s ",Q[front].msg);
		front=(front+1)%N;
	}
	return 0;
}


static void display(void)
{
	int i = front;
	if(front==-1 && rear == -1)
	{
		printk(KERN_INFO "que is empty\n");
	}
	else
	{
		printk(KERN_INFO "que is = \n");
		while(i != rear)
		{
				printk(KERN_INFO "que = %s ",Q[i].msg);
				strcpy(device_buffer,Q[i].msg);
		//strncpy(Q[i].msg,message,sizeof(Q[i].msg)-1);
		//Q[i].msg[sizeof(Q[i].msg)-1]='\0';
				printk(KERN_INFO "device buffer in display = %s ",Q[i].msg);
				i=((i+1)%N);
			}
	}
}

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
/*
 * In below function I faced the issue with change in size given from the userspace.
 * It was due to offset condition, removed that and the problem was fixed.
 */


// Called when data is read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    size_t len = strlen(device_buffer);

    //remove the element
    deque();
     printk(KERN_INFO "main deque device_buffer = %s\n",device_buffer);
     char temp[200];
     printk(KERN_INFO "main deque temp = %s\n",temp);
    if (copy_to_user(user_buffer,device_buffer, size)) {
	    return -EFAULT;
    }
     printk(KERN_INFO " main use_buffer deque = %s\n",user_buffer);

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


     printk(KERN_INFO "device_buffer = %s\n",device_buffer);
    // Adding the received message to the que//
    enque();
    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", size);
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
MODULE_AUTHOR("Prince Suresh");
MODULE_DESCRIPTION("A Simple Linux Device Driver");
