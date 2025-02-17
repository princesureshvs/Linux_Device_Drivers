#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DEVICE_NAME "ioctl_device"  // Name of the device (used in /dev)
#define CLASS_NAME "ioctl_class"    // Class name for the device

// IOCTL Command Definitions
#define IOCTL_GET_DATA _IOR('i', 1, int)  // Command to get data from the kernel (input: none, output: int)
#define IOCTL_SET_DATA _IOW('i', 2, int)  // Command to set data to the kernel (input: int, output: none)

static int device_open = 0;  // Variable to track if the device is already opened
static int ioctl_data = 0;   // Variable to store the data for IOCTL communication

// File operation: open the device
static int dev_open(struct inode *inodep, struct file *filep)
{
    if (device_open)  // Check if the device is already opened
        return -EBUSY;  // Return "Device Busy" if already opened

    device_open++;  // Increment the device open count
    pr_info("Device opened\n");
    return 0; 
}

// File operation: release the device
static int dev_release(struct inode *inodep, struct file *filep)
{
    device_open--;  // Decrement the device open count
    pr_info("Device closed\n");
    return 0;  
}

// File operation: IOCTL (Input/Output Control) for handling device-specific commands
static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    int temp;
    switch (cmd) {
    case IOCTL_GET_DATA:  // Command to get data from the kernel
        if (copy_to_user((int __user *)arg, &ioctl_data, sizeof(ioctl_data))) {
            return -EFAULT;  
        }
        pr_info("IOCTL_GET_DATA: Data sent to user: %d\n", ioctl_data);
        break;
    case IOCTL_SET_DATA:  // Command to set data to the kernel
        if (copy_from_user(&temp, (int __user *)arg, sizeof(temp))) {
            return -EFAULT;  
        }
        ioctl_data = temp;  // Update the kernel's data with the received value
        pr_info("IOCTL_SET_DATA: Data received from user: %d\n", ioctl_data);
        break;
    default:
        return -EINVAL;  // Invalid command
    }
    return 0; 
}

// File operations structure to map file operations to functions
static struct file_operations fops = {
    .owner = THIS_MODULE,           
    .open = dev_open,              
    .release = dev_release,       
    .unlocked_ioctl = dev_ioctl,  
};

// Major number for the device and device class
static int major_number;
static struct class *ioctl_class;
static struct device *ioctl_device;

// Module initialization function
static int __init ioctl_driver_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &fops);  // Register character device with a dynamic major number
    if (major_number < 0) {
        pr_err("Failed to register a major number\n");
        return major_number;  // Return error if registration fails
    }
    pr_info("Registered with major number %d\n", major_number);

    // Create a device class
    ioctl_class = class_create(CLASS_NAME);
    if (IS_ERR(ioctl_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);  // Unregister if class creation fails
        pr_err("Failed to register device class\n");
        return PTR_ERR(ioctl_class);  // Return error if class creation fails
    }
    pr_info("Device class registered\n");

    // Create the device file in /dev
    ioctl_device = device_create(ioctl_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(ioctl_device)) {
        class_destroy(ioctl_class);  // Clean up class if device creation fails
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_err("Failed to create the device\n");
        return PTR_ERR(ioctl_device);  // Return error if device creation fails
    }
    pr_info("Device created successfully\n");

    return 0;  // Successfully initialized the driver
}

// Module cleanup function
static void __exit ioctl_driver_exit(void)
{
    device_destroy(ioctl_class, MKDEV(major_number, 0));  // Destroy the device file
    class_unregister(ioctl_class);  // Unregister the device class
    class_destroy(ioctl_class);    // Destroy the class
    unregister_chrdev(major_number, DEVICE_NAME);  // Unregister the character device
    pr_info("IOCTL driver unloaded\n");
}

// Register initialization and cleanup functions
module_init(ioctl_driver_init);
module_exit(ioctl_driver_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("prince");       
MODULE_DESCRIPTION("A simple IOCTL Linux driver");  
MODULE_VERSION("1.0");            
