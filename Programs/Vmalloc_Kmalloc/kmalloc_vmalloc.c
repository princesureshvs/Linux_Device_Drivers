#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>  // For kmalloc
#include <linux/vmalloc.h> // For vmalloc
#include <linux/io.h>  // For ioremap (if needed for I/O operations)
#include <linux/fs.h>  // For character device registration

#define KMALLOC_SIZE 1024    // 1 KB for kmalloc
#define VMALLOC_SIZE 10240   // 10 KB for vmalloc

// Pointers to the allocated memory
void *kmalloc_memory = NULL;
void *vmalloc_memory = NULL;

// Module initialization function
static int __init memory_alloc_init(void)
{
    printk(KERN_INFO "Initializing memory allocation module\n");

    // Allocate memory using kmalloc (physically contiguous)
    kmalloc_memory = kmalloc(KMALLOC_SIZE, GFP_KERNEL);
    if (!kmalloc_memory) {
        printk(KERN_ERR "Failed to allocate memory using kmalloc\n");
        return -ENOMEM;
    }
    printk(KERN_INFO "Memory allocated using kmalloc at address: %p\n", kmalloc_memory);

    // Allocate memory using vmalloc (virtually contiguous)
    vmalloc_memory = vmalloc(VMALLOC_SIZE);
    if (!vmalloc_memory) {
        printk(KERN_ERR "Failed to allocate memory using vmalloc\n");
        kfree(kmalloc_memory);  // Free kmalloc memory
        return -ENOMEM;
    }
    printk(KERN_INFO "Memory allocated using vmalloc at address: %p\n", vmalloc_memory);

    return 0; // Successful initialization
}

// Module exit function
static void __exit memory_alloc_exit(void)
{
    printk(KERN_INFO "Exiting memory allocation module\n");

    // Free memory allocated by kmalloc
    if (kmalloc_memory) {
        kfree(kmalloc_memory);
        printk(KERN_INFO "Memory freed using kmalloc\n");
    }

    // Free memory allocated by vmalloc
    if (vmalloc_memory) {
        vfree(vmalloc_memory);
        printk(KERN_INFO "Memory freed using vmalloc\n");
    }
}

// Register the module's initialization and exit functions
module_init(memory_alloc_init);
module_exit(memory_alloc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prince");
MODULE_DESCRIPTION("A simple example module using kmalloc and vmalloc.");
