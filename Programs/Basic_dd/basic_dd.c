#include<linux/module.h>
#include<linux/kernel.h>

static int __init basic_driver_init(void)
{
	printk(KERN_INFO "Basic Driver Loaded\n");
	return 0;
}

static void __exit basic_driver_exit(void)
{

	printk(KERN_INFO "Basic Driver unloaded\n");
}

module_init(basic_driver_init);
module_exit(basic_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRINCE");
MODULE_DESCRIPTION("A Basic Linux Device Driver\n");
