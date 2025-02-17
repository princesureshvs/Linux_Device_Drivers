#include <linux/init.h>          // Initialization macros and functions for kernel modules
#include <linux/module.h>        // Core module functions like module_init(), module_exit(), and licensing
#include <linux/kernel.h>        // Kernel-level macros and functions for logging and message output (e.g., pr_info, pr_err)
#include <linux/fs.h>            // File system-related functions (not heavily used in this code, but included for sysfs)
#include <linux/device.h>        // Device model, includes functionality for working with device attributes (such as sysfs)
#include <linux/gpio.h>          // GPIO (General Purpose Input/Output) handling functions (e.g., gpio_request, gpio_direction_output)
#include <linux/sysfs.h>         // Sysfs (virtual file system) handling, includes functions for creating and managing sysfs files
#include <linux/kobject.h>       // Kobject management functions (e.g., kobject_create_and_add, kobject_put) for creating sysfs groups
#include <linux/slab.h>          // Memory allocation functions (like kmalloc, kfree)

MODULE_LICENSE("GPL");          // License type of the module (GPL - General Public License)
MODULE_AUTHOR("prince");     // Author of the module
MODULE_DESCRIPTION("Sysfs example for controlling GPIO");  // Short description of what the module does
MODULE_VERSION("1.0");          // Module version, useful for versioning of kernel modules

#define LED_GPIO 17  // GPIO pin for the LED

// Sysfs attributes
static struct kobject *example_kobject;  // Pointer to kobject representing our sysfs entry
static int led_state = 0;  // Variable to store the current state of the LED (0 = OFF, 1 = ON)

// GPIO initialization
static int gpio_init(void) {
	int ret;

	// Request GPIO pin for LED control
	ret = gpio_request(LED_GPIO, "sysfs_led");  // Request GPIO pin 17, with a label "sysfs_led"
	if (ret) {
		pr_err("Failed to request GPIO %d\n", LED_GPIO);  // Error if the GPIO request fails
		return ret;
	}

	// Set GPIO pin direction to output (0 = OFF, 1 = ON)
	ret = gpio_direction_output(LED_GPIO, 0);  // Set the pin as output, initially turned off (0)
	if (ret) {
		pr_err("Failed to set GPIO %d as output\n", LED_GPIO);  // Error if direction setup fails
		gpio_free(LED_GPIO);  // Free the GPIO pin if setup fails
		return ret;
	}

	pr_info("GPIO %d initialized as output\n", LED_GPIO);  // Success message
	return 0;
}

// GPIO cleanup (free GPIO resources)
static void gpio_cleanup(void) {
	gpio_set_value(LED_GPIO, 0); // Ensure the LED is turned off before freeing the GPIO pin
	gpio_free(LED_GPIO);         // Free the GPIO pin to release the resources
	pr_info("GPIO %d freed\n", LED_GPIO);  // Success message after cleanup
}

// Sysfs attribute: Show the current LED state
static ssize_t led_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
	return sprintf(buf, "%d\n", led_state);  // Return the current LED state (0 or 1)
}

// Sysfs attribute: Store a new LED state (ON/OFF)
static ssize_t led_state_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
	int value;

	// Parse the input value and ensure it's either 0 or 1
	if (kstrtoint(buf, 10, &value) != 0 || (value != 0 && value != 1)) {
		pr_err("Invalid value, please write 0 or 1\n");  // Print error if the value is not 0 or 1
		return -EINVAL;  // Return error code for invalid input
	}

	led_state = value;  // Update the LED state (0 or 1)
	gpio_set_value(LED_GPIO, led_state);  // Set the GPIO pin to the new state (ON/OFF)
	pr_info("LED state set to %d\n", led_state);  // Log the new LED state

	return count;  // Return the number of bytes written
}

// Declare the sysfs attribute (led_state) with read and write permissions (0660)
static struct kobj_attribute led_state_attribute =
__ATTR(led_state, 0660, led_state_show, led_state_store);  // Attribute for reading/writing LED state

// Initialize the module
static int __init sysfs_example_init(void) {
	int ret;

	pr_info("Initializing sysfs example module\n");

	// Initialize GPIO for LED control
	ret = gpio_init();
	if (ret) {
		pr_err("Failed to initialize GPIO\n");
		return ret;
	}

	// Create a kobject for the sysfs interface (this will be a folder in sysfs)
	example_kobject = kobject_create_and_add("sysfs_example", kernel_kobj);  // Create kobject under the kernel's sysfs
	if (!example_kobject) {
		pr_err("Failed to create sysfs kobject\n");  // Error if kobject creation fails
		gpio_cleanup();  // Clean up GPIO if kobject creation fails
		return -ENOMEM;  // Return memory error if kobject creation fails
	}

	// Create the sysfs file for LED state control (linked to led_state_attribute)
	ret = sysfs_create_file(example_kobject, &led_state_attribute.attr);  // Create the sysfs file 'led_state'
	if (ret) {
		pr_err("Failed to create sysfs file\n");  // Error if sysfs file creation fails
		kobject_put(example_kobject);  // Clean up the kobject
		gpio_cleanup();  // Clean up GPIO
		return ret;  // Return error
	}

	pr_info("Sysfs example module initialized\n");
	return 0;  // Return 0 to indicate successful initialization
}

// Exit the module and clean up resources
static void __exit sysfs_example_exit(void) {
	pr_info("Exiting sysfs example module\n");

	// Remove the sysfs file and release the kobject
	sysfs_remove_file(example_kobject, &led_state_attribute.attr);  // Remove the sysfs file 'led_state'
	kobject_put(example_kobject);  // Release the kobject (this frees the memory used by the sysfs object)

	// Clean up GPIO to release the resources
	gpio_cleanup();

	pr_info("Sysfs example module exited\n");
}

// Register initialization and exit functions for the module
module_init(sysfs_example_init);  // Register the module initialization function
module_exit(sysfs_example_exit);  // Register the module exit function
