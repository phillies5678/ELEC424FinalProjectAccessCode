#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
// Add interrupts here
#include <linux/interrupt.h>
#include <linux/ktime.h>
/* YOU WILL NEED OTHER HEADER FILES */

/* YOU WILL HAVE TO DECLARE SOME VARIABLES HERE */

// Create our gpio_descs
struct gpio_desc *buttondesc;
// Int for our interrupt
int buttonIRQ;

// Current time
ktime_t currTime;
// Previous time
ktime_t prevTime;
//Different
int diff;

// Print out the difference to a file
module_param(diff, int, S_IRUGO);

/* ADD THE INTERRUPT SERVICE ROUTINE HERE */

/**
 * @brief Interrupt service routine is called, when interrupt is triggered
 */
static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs) {
	// Set if we trigger interupt
	
	//Count how long its been since last ESC has been triggered
	currTime = ktime_get();
	int temp = ktime_to_ns(currTime - prevTime) / 1000000;
	if (temp > 1) {
		diff = temp;
	}

	printk("Received difference of: %d\n", diff);

	prevTime = currTime;

	// finished handling
	return (irq_handler_t) IRQ_HANDLED; 
}

// probe function
static int led_probe(struct platform_device *pdev)
{
	printk("Led Probing\n");
	// Set up or led and button with devm_gpiod_get
	buttondesc = devm_gpiod_get(&pdev->dev, "userbutton", GPIOD_IN);
	// Set IRQ with gpiod_to_irq
	buttonIRQ = gpiod_to_irq(buttondesc);

	// We are not handling intterupts
	printk("Interupt handling\n");

	
	//Open file

	// Try to handle interupts
	if (request_irq(buttonIRQ, (irq_handler_t) gpio_irq_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0){
		// If we end up here we close the program
		printk("Can not interupt here");
		return -1;
	}

	// Set up debouncing
	printk("Debouncing\n");
	gpiod_set_debounce(buttondesc, 1000000);


	// We are done here
	printk("Finished\n");
	return 0;
}

// remove function
static int led_remove(struct platform_device *pdev)
{
	// Free the buttonIRQ
	free_irq(buttonIRQ, NULL);
	printk("Freed irq_num\n");
	return 0;
}

static struct of_device_id matchy_match[] = {
	// our driver name is hello
    {.compatible = "hello"},
    {/* leave alone - keep this here (end node) */},
};

// platform driver object
static struct platform_driver adam_driver = {
	// Set up functions here
	.probe	 = led_probe,
	.remove	 = led_remove,
	.driver	 = {
	       .name  = "The Rock: this name doesn't even matter",
	       .owner = THIS_MODULE,
	       .of_match_table = matchy_match,
	},
};

// Set up module_platform_driver
module_platform_driver(adam_driver);

// Module stuff :)
MODULE_DESCRIPTION("424\'s finest");
MODULE_AUTHOR("GOAT");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:adam_driver");
