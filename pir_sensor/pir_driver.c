#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/gpio.h>
#include<linux/fs.h>

#define DEVICE_NAME "pir_sensor"
#define GPIO_PIR_OUTPUT 530
#define GPIO_LED 529

static int gpio_open(struct inode *inode,struct file *file);
static int gpio_close(struct inode *inode,struct file *file);
static ssize_t gpio_read(struct file *file,char __user *buf,size_t size,loff_t *offset);
static ssize_t gpio_write(struct file *file,const char __user *buf,size_t size,loff_t *offset);

//static int pre_pir_out=0,cur_pir_out=0;
static struct file_operations fops={
	.owner=THIS_MODULE,
	.open=gpio_open,
	.release=gpio_close,
        .write=gpio_write,
	.read=gpio_read,
};


static int gpio_open(struct inode *inode,struct file *file)
{
	pr_info("GPIO module opened successfully\n");
	return 0;
}

static int gpio_close(struct inode *inode,struct file *file)
{
	gpio_set_value(GPIO_LED,0);
	pr_info("GPIO module closed successfully\n");
	return 0;
}

static ssize_t gpio_read(struct file *file,char __user *buf,size_t size,loff_t *offset)
{
	int value=gpio_get_value(GPIO_PIR_OUTPUT);
	char buffer[2];
	snprintf(buffer,2,"%d",value);
	buffer[1]='\0';

	if(copy_to_user(buf,buffer,2))
	{
	pr_info("Failed to send data to user\n");
	return -EFAULT;
	}	

	pr_info("GPIO device read:%d\n",value);
	return 2;
}

static ssize_t gpio_write(struct file *file,const char __user *buf,size_t size,loff_t *offset)
{
	char buffer[2];
	if(copy_from_user(buffer,buf,size))
	{
		pr_info("pir_device: Data not received from user\n");
		return -EFAULT;
	}
	
	buffer[size]='\0';	
	int value=simple_strtol(buffer,NULL,10);

	if(value==0 || value==1)
	{
		gpio_set_value(GPIO_LED,value);
		pr_info("GPIO writes:%d\n",value);
	}
	else
	{
	pr_info("Invalid Value: GPIO accepts only 0 or 1\n");
	return -EINVAL;
	}

	return size;
}

static int __init gpio_init(void)
{
	int ret;

	ret=gpio_request(GPIO_PIR_OUTPUT,"GPIO_DRIVER");
	if(ret)
	{
		pr_info("failed to request GPIO pin: %d\n",GPIO_PIR_OUTPUT);
		return ret;
	}
	ret=gpio_request(GPIO_LED,"GPIO_DRIVER");
	if(ret)
	{
		pr_info("failed to request GPIO pin: %d\n",GPIO_LED);
		return ret;
	}

	ret=gpio_direction_input(GPIO_PIR_OUTPUT);
	if(ret)
	{
		pr_info("failed to set the GPIO direction for the pin: %d\n",GPIO_PIR_OUTPUT);
		gpio_free(GPIO_PIR_OUTPUT);
		return ret;
	}
	ret=gpio_direction_output(GPIO_LED,0);
	if(ret)
	{
		pr_info("failed to set the GPIO direction for the pin: %d\n",GPIO_LED);
		gpio_free(GPIO_LED);
		return ret;
	}

	ret=register_chrdev(0,DEVICE_NAME,&fops);
	if(ret<0)
	{
		pr_info("failed to register PIR driver");
		gpio_free(GPIO_LED);
		gpio_free(GPIO_PIR_OUTPUT);
		return ret;
	}

	pr_info("PIR driver loaded with major no: %d\n",ret);
	return 0;
}

static void __exit gpio_exit(void)
{
	unregister_chrdev(0,DEVICE_NAME);
	gpio_free(GPIO_LED);
	gpio_free(GPIO_PIR_OUTPUT);
	pr_info("PIR driver unloaded\n");
}	

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prince");
MODULE_DESCRIPTION("PIR sensor interfaced with LED");
