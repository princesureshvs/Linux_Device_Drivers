#include <linux/module.h>   
#include <linux/kernel.h>    
#include <linux/init.h>      
#include <linux/interrupt.h> 

/* IRQ number for the keyboard, to find number you can check /proc/interrupts */

#define KEYBOARD_IRQ 1 // IRQ number for the keyboard, typical for x86 systems

// Interrupt handler function
static irqreturn_t keyboard_irq_handler(int irq, void *dev_id)
{
	printk(KERN_INFO "Keyboard interrupt received!\n");  
	return IRQ_HANDLED;  // Return IRQ_HANDLED to indicate the interrupt has been processed
}

// Module initialization function
static int __init keyboard_interrupt_init(void)
{
	int result;
	printk(KERN_INFO "Initializing the Keyboard Interrupt Driver\n"); 

	// request_irq binds the IRQ to the interrupt handler
	result = request_irq(KEYBOARD_IRQ, keyboard_irq_handler, IRQF_SHARED, 
			"keyboard_interrupt_driver", (void *)keyboard_irq_handler);
	if (result) {
		printk(KERN_ERR "TechDhaba: Failed to request IRQ %d, error: %d\n", KEYBOARD_IRQ, result);
		return result; 
	}

	printk(KERN_INFO "Keyboard Interrupt Driver successfully loaded\n");
	return 0;  
}

// Module cleanup function
static void __exit keyboard_interrupt_exit(void)
{
	// Release the IRQ and clean up the interrupt handler
	free_irq(KEYBOARD_IRQ, (void *)keyboard_irq_handler);
	printk(KERN_INFO "Keyboard Interrupt Driver unloaded\n");
}

// Register module initialization and cleanup functions
module_init(keyboard_interrupt_init);
module_exit(keyboard_interrupt_exit);

// Module metadata
MODULE_LICENSE("GPL");               
MODULE_AUTHOR("prince");             
MODULE_DESCRIPTION("Keyboard Interrupt Driver");

