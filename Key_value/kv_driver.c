#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "kv_logger"
#define BUFFER_SIZE 1024

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;
static int count=0;

static struct key_value{
	int key;
	int value;
}KV[5];


// Logic to get key and value as an integer.
 
static void add_keyvalue(void)
{
        int key1=0,value1=0,flag=0;
        char *buff = device_buffer;

        while(*buff != '\0')
        {
                if(*buff >= '0' && *buff <= '9')
                {
                        if(flag == 0)
                        {
                                key1 = key1*10 + (*buff-48);
                        }
                        if(flag == 1)
                        {
                                value1 = value1*10 + (*buff-48);
                        }
                }
                if(*buff == ' ')
                {
                        flag=1;
                }
                buff++;
        }

	//printk(KERN_INFO "key1= %d\n", key1);
	//printk(KERN_INFO "value1= %d\n", value1);
	//printk(KERN_INFO "count= %d\n", count);
	KV[count].key = key1;
	KV[count].value = value1;
	//printk(KERN_INFO "key= %d\n", KV[count].key);
	//printk(KERN_INFO "value= %d\n", KV[count].value);
	count++;

}

static void return_value(void)
{
        int index=0,key=0;
	char *check = device_buffer;
	while(*check != '\0' )
	{
             key=key*10+(*check - 48);
	     check++;
	}
	//printk(KERN_INFO "index= %d\n", index);
	check = device_buffer;
	while(*check != '\0' )
	{
		if(KV[index].key == key)
		break;
		index++;
	}
	//printk(KERN_INFO "index= %d\n", index);
        //sprintf(device_buffer,"%d %d",KV[index].key,KV[index].value);
	sprintf(device_buffer,"%d",KV[index].value);
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

// Called when data is read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
	size_t len = strlen(device_buffer);

	if (*offset >= len) // Check if all data has been read
		return 0;

	if (size > len - *offset) // Adjust size to avoid reading beyond buffer
		size = len - *offset;
	// copying the key value received from userbuffer to device buffer
	if (copy_from_user(device_buffer, user_buffer, size)) {
		return -EFAULT;
	}

	printk(KERN_INFO "device_buffer= %s\n", device_buffer);
	printk(KERN_INFO "usr_buffer= %s\n", user_buffer);

	/* returning the stored value */	
	return_value();
        // The value in device_buffer is copied to user_buffer
	printk(KERN_INFO "device_buffer= %s\n", device_buffer);
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
	/* adding the key and value */
	add_keyvalue();
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
