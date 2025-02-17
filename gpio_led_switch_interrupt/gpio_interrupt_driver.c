/* In this code we are not using any file operations, as from the user space we are not doing any file operations */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>//used for gpio descriptor

#define GPIO_BUTTON_PIN 529 // GPIO pin for the button (input), to see this use gpioinfo or cat /sys/kernel/debug/gpio
#define GPIO_LED_PIN 530    // GPIO pin for the LED (output)

/* Here no struct file_operations is not used */

static unsigned int irq_number;         // IRQ number for the button GPIO
static bool led_state = false;          // LED state (ON/OFF)
static struct gpio_desc *button_desc;   // GPIO descriptor for the button
static struct gpio_desc *led_desc;      // GPIO descriptor for the LED

// Interrupt handler function
static irqreturn_t gpio_irq_handler(int irq, void *dev_id) {
    pr_info("GPIO Interrupt triggered!\n");

    // Toggle LED state or perform your desired operation
    led_state = !led_state;
    gpiod_set_value(led_desc, led_state);  // Use the GPIO descriptor to set value

    return IRQ_HANDLED;
}

// Marks this function as the module initialization function.
static int __init gpio_interrupt_init(void) {
    int ret;

    pr_info("GPIO Interrupt Module Loading\n");

    // Converts the button GPIO pin number to a descriptor.
    button_desc = gpio_to_desc(GPIO_BUTTON_PIN);
    led_desc = gpio_to_desc(GPIO_LED_PIN);

    if (!button_desc || !led_desc) {
        pr_err("Failed to get GPIO descriptors for GPIO pins\n");
        return -ENODEV;
    }

    // Configure the button GPIO as input
    ret = gpiod_direction_input(button_desc);
    if (ret) {
        pr_err("Failed to set button GPIO direction\n");
        return ret;
    }

    // Sets the LED GPIO as an output and initializes it with led_state.
    ret = gpiod_direction_output(led_desc, led_state);
    if (ret) {
        pr_err("Failed to set LED GPIO direction\n");
        return ret;
    }

    // Maps the button GPIO pin to an IRQ number.
    irq_number = gpiod_to_irq(button_desc);
    if (irq_number < 0) {
        pr_err("Failed to get IRQ number for button GPIO: %d\n", GPIO_BUTTON_PIN);
        return irq_number;
    }

    // Registers an interrupt handler for the IRQ.
    ret = request_irq(
        irq_number,                // IRQ number
        gpio_irq_handler,          // Interrupt handler function
        IRQF_TRIGGER_RISING,       // Trigger on rising edge
        "gpio_interrupt",          // Interrupt name
        NULL                       // Device ID (optional)
    );

    if (ret) {
        pr_err("Failed to request IRQ: %d\n", irq_number);
        return ret;
    }

    pr_info("GPIO Interrupt Module Loaded Successfully\n");
    return 0;
}

// Marks this function as the cleanup function for the module.
static void __exit gpio_interrupt_exit(void) {
    pr_info("GPIO Interrupt Module Unloading\n");

    // Free the IRQ line
    free_irq(irq_number, NULL);    

    // Unexport GPIOs (if previously exported for userspace access)
   // gpiod_unexport(button_desc);
    //gpiod_unexport(led_desc);

    pr_info("GPIO Interrupt Module Unloaded Successfully\n");
}

module_init(gpio_interrupt_init);
module_exit(gpio_interrupt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prince suresh");
MODULE_DESCRIPTION("A simple GPIO interrupt driver for Raspberry Pi");
MODULE_VERSION("1.0");
