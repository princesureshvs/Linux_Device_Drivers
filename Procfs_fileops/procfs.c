#include <linux/module.h>      // Provides macros for module initialization and cleanup
#include <linux/kernel.h>      // Includes kernel-level functions like pr_info() and pr_err()
#include <linux/init.h>        // Provides macros for module initialization and cleanup functions (__init and __exit)
#include <linux/proc_fs.h>     // Provides functions to interact with the /proc filesystem
#include <linux/uaccess.h>     // Provides functions for copying data between user space and kernel space (e.g., copy_from_user)

#define PROC_FILENAME "my_proc_file"  // Name of the proc file
#define PROC_MAX_SIZE 1024            // Maximum size of data in the proc file

static char proc_data[PROC_MAX_SIZE];  // Buffer to store data
static struct proc_dir_entry *proc_file;  // Pointer to the proc file entry

/* Function prototypes */
static int proc_open(struct inode *inode, struct file *file);
static int proc_close(struct inode *inode, struct file *file);
static ssize_t proc_read(struct file *file, char __user *userbuffer, size_t len, loff_t *offset);
static ssize_t proc_write(struct file *file, const char __user *userbuffer, size_t len, loff_t *offset);

/* File operations */
static const struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_release = proc_close,
    .proc_read = proc_read,
    .proc_write = proc_write,
};

// Opening the procfs file
static int proc_open(struct inode *inode, struct file *file)
{
    pr_info("procfs file opened\n");
    return 0;  
}

// Closing the procfs file
static int proc_close(struct inode *inode, struct file *file)
{
    pr_info("procfs file closed\n");
    return 0;  
}

// Reading from the procfs file
static ssize_t proc_read(struct file *file, char __user *userbuffer, size_t len, loff_t *offset)
{
    // Use simple_read_from_buffer to copy data to user space
    return simple_read_from_buffer(userbuffer, len, offset, proc_data, strlen(proc_data));
}

// Writing to the procfs file
static ssize_t proc_write(struct file *file, const char __user *userbuffer, size_t len, loff_t *offset)
{
    if (len > PROC_MAX_SIZE)  // Ensure data size is within limit
        return -EINVAL;  

    if (copy_from_user(proc_data, userbuffer, len))  // Copy user data to proc_data
    {
        return -EFAULT;  
    }

    proc_data[len] = '\0';  // Null-terminate the string
    return len;  // Return the number of bytes written
}

// Initialization function
static int __init procfs_example_init(void)
{
    proc_file = proc_create(PROC_FILENAME, 0666, NULL, &proc_fops);  // Create proc file
    if (proc_file < 0)  
    {
        pr_err("Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;  
    }

    pr_info("procfs_example module loaded. FILE:/proc/%s\n", PROC_FILENAME);  
    return 0;  // Success
}

// Cleanup function
static void __exit procfs_example_exit(void)
{
    proc_remove(proc_file);  // Remove the proc file
    pr_info("Module unloaded...\n"); 
}

module_init(procfs_example_init);  
module_exit(procfs_example_exit);  

MODULE_LICENSE("GPL");             
MODULE_AUTHOR("prince");             
MODULE_DESCRIPTION("A simple procfs example module");   
