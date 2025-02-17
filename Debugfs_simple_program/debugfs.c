#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/uaccess.h>
#include<linux/debugfs.h>

#define MODULE_NAME "debugfs_example"

static struct dentry *dir,*file;
static int debugfs_value = 0;

/* File operations prototypes */
static int debugfs_open(struct inode *inode, struct file *file);
static int debugfs_close(struct inode *inode,struct file *file);
static ssize_t debugfs_read(struct file *file,char __user *user_buffer,size_t count,loff_t *position);
static ssize_t debugfs_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *position);

/* File operations structure */
static const struct file_operations fops = {
	.open = debugfs_open,
	.release = debugfs_close,
	.read = debugfs_read,
	.write = debugfs_write,
};

/* Module Open */
static int debugfs_open(struct inode *inode,struct file *file)
{
	pr_info("debugfs_example file opened\n");
	return 0;
}

/* Module Close */
static int debugfs_close(struct inode *inode, struct file *file)
{
	pr_info("Debugfs_example file closed\n");
	return 0;
}

/* Module read */
static ssize_t  debugfs_read(struct file *file, char __user *user_buffer, size_t count, loff_t *position)
{
	char buffer[64];
	int len = snprintf(buffer, sizeof(buffer), "%d\n", debugfs_value);
	return simple_read_from_buffer(user_buffer, count, position, buffer, len);
}

/* Module write */
static ssize_t debugfs_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *position)
{
	char buffer[64];

	if (count >= sizeof(buffer))
		return -EINVAL;

	if (copy_from_user(buffer, user_buffer, count))
		return -EFAULT;

	buffer[count] = '\0';
	if (kstrtoint(buffer, 10, &debugfs_value))
		return -EINVAL;

	return count;

}

/* Module Initialisation */
static int __init debugfs_example_init(void)
{
	dir = debugfs_create_dir(MODULE_NAME,NULL);//Create directory in the /sys/kernel/debug
	if(!dir)
	{
		pr_err("Failed to create the debugfs directory\n");
		return -ENOMEM;
	}

	file = debugfs_create_file("value",0777,dir,NULL,&fops);//Create file with name value under /sys/kernel/debug/debugfs_example/value
	if(!file)
	{
		pr_err("Failed to create debugfs file\n");
		debugfs_remove_recursive(dir);
		return -ENOMEM;
	}
	pr_info("Debugfs_example module loaded\n");
	return 0;
}

/* Module Exit */
static void __exit debugfs_example_exit(void)
{
	debugfs_remove_recursive(dir);//Remove all debugfs entries
	pr_info("debugfs_example module unloaded\n");
}

module_init(debugfs_example_init);
module_exit(debugfs_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRINCE");
MODULE_DESCRIPTION("A simple Debugfs Example");

