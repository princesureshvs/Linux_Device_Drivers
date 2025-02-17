#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/fcntl.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#define DEVICE_NAME "uart_device"
#define UART_PORT "/dev/ttyAMA0"

/* Function prototypes */
static int uart_open(struct inode *inode, struct file *file);
static int uart_close(struct inode *inode, struct file *file);
static ssize_t uart_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offset);
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);

/* File operations structure */
static struct file_operations uart_fops = {
	.owner = THIS_MODULE,
	.open = uart_open,
	.release = uart_close,
	.read = uart_read,
	.write = uart_write,
};

/* Global file pointer for UART device */
static struct file *uart_file = NULL;

/* Open function */
static int uart_open(struct inode *inode, struct file *file) {
	uart_file = filp_open(UART_PORT, O_RDWR | O_NOCTTY | O_NONBLOCK, 0);//O_NONBLOCK removed here 
	if (IS_ERR(uart_file)) {
		pr_err("Failed to open UART device %s\n", UART_PORT);
		return PTR_ERR(uart_file);
	}

	pr_info("UART device opened successfully\n");
	return 0;
}

/* Close function */
static int uart_close(struct inode *inode, struct file *file) {
	if (uart_file && !IS_ERR(uart_file)) {
		filp_close(uart_file, NULL);
		pr_info("UART device closed\n");
	}
	return 0;
}

/* Read function */
static ssize_t uart_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offset) {
	char kernel_buffer[256]; // Temporary kernel buffer
	ssize_t bytes_read;
	//offset=0;//debugging purpose

	if (!uart_file || IS_ERR(uart_file)) {
		pr_err("UART device is not opened\n");
        return -ENODEV;
	}
	/* Read from UART device into kernel buffer */
	msleep(100);//debugging
	bytes_read = kernel_read(uart_file,kernel_buffer,min(len,sizeof(kernel_buffer)),offset);//uart_file,kernel_buffer
	if (bytes_read < 0) {

		if(bytes_read == -EAGAIN)
		{
			pr_err("No data available in uart buffer\n");
		}
		else
		{
			pr_err("failed to read from uart device %zd\n",bytes_read);
		}
		//pr_err("Failed to read from UART device\n");
		return bytes_read;
	}

	pr_info("kernel buffer in read =%s\n",kernel_buffer);// for debugging purpose

	/* Copy data from kernel buffer to user space */
	if (copy_to_user(user_buffer, kernel_buffer, bytes_read)) {
		pr_err("Failed to copy data to user space\n");
		return -EFAULT;
	}

	pr_info("Read %zd bytes from UART device\n", bytes_read);
	return bytes_read;
}

/* Write function */
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
	char kernel_buffer[256]; // Temporary kernel buffer
	ssize_t bytes_written;

	if (!uart_file || IS_ERR(uart_file)) {
		pr_err("UART device is not opened\n");
		return -ENODEV;
	}

	/* Copy data from user space to kernel buffer */
	if (copy_from_user(kernel_buffer, buf, min(len, sizeof(kernel_buffer)))) {
		pr_err("Failed to copy data from user space\n");
		return -EFAULT;
	}

	/* Write data from kernel buffer to UART device */
	bytes_written = kernel_write(uart_file, kernel_buffer, len, offset);
	if (bytes_written < 0) {
		pr_err("Failed to write to UART device\n");
		return bytes_written;
	}
        pr_info("kernel_buffer in write to uart_file=%s\n",kernel_buffer);
	pr_info("Written %zd bytes to UART device\n", bytes_written);
	return bytes_written;
}

/* Module initialization function */
static int __init uart_init(void) {
	int ret;

	pr_info("Initializing UART driver\n");

	/* Register character device */
	ret = register_chrdev(0, DEVICE_NAME, &uart_fops);
	if (ret < 0) {
		pr_err("Failed to register UART device\n");
		return ret;
	}

	pr_info("UART driver registered successfully with major number %d\n", ret);
	return 0;
}

/* Module cleanup function */
static void __exit uart_exit(void) {
	pr_info("Cleaning up UART driver\n");

	/* Unregister character device */
	unregister_chrdev(0, DEVICE_NAME);
}
module_init(uart_init);
module_exit(uart_exit);
MODULE_LICENSE("GPL");

