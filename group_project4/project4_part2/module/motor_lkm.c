#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/gpio.h>                       // for the GPIO functions 
#include <linux/interrupt.h>                  // for the IRQ code
#include <linux/fs.h>

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Antoine DUPUIS"); 
MODULE_DESCRIPTION("Motor driver reading state of rotation and implement PWM"); 
MODULE_VERSION("0.1");

#define  DEVICE_MAJOR 234         ///< Requested device node major number or 0 for dynamic allocation
#define  DEVICE_NAME "motor"   ///< In this implementation, the device name has nothing to do with the name of the device in /dev. You must use mknod to create the device node in /dev

// static unsigned int gpioLED = 18;             // pin 18 (GPIO18) 
static unsigned int gpioMotorInput = 17;          // pin 17 (GPIO17) 
static unsigned int irqNumber;                // share IRQ num within file 
int motorInputState = 0;

// static bool         ledOn = 0;                // used to invert state of LED
static int   majorNumber;        ///< Stores the device number -- determined automatically
static int   numberOpens = 0;    ///< Counts the number of times the device is opened
static char *name = "world";

module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

// The prototype functions for the character driver -- must come before the struct definition
static int      erpi_gpio_open(struct inode *, struct file *);
static ssize_t  erpi_gpio_write(struct file *, const char *, size_t, loff_t *);
static int      erpi_gpio_release(struct inode *, struct file *);
static ssize_t  erpi_gpio_read(struct file *file, char *buffer, size_t length, loff_t *offset);

static struct file_operations fops =
{
   .open = erpi_gpio_open,
   .write = erpi_gpio_write,
   .release = erpi_gpio_release,
   .read = erpi_gpio_read,
};

static ssize_t erpi_gpio_write(struct file * file, const char *buf, size_t count, loff_t *ppos)
{
   printk("mydev write: accepting %zu bytes from the user\n", count);
   return count;
}

static int erpi_gpio_release(struct inode *inodep, struct file *filep){
//    printk(KERN_INFO "mydev: Device successfully closed\n");
   return 0;
}

static int erpi_gpio_open(struct inode *inodep, struct file *filep){
   numberOpens++;
//    printk(KERN_INFO "mydev: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

static ssize_t erpi_gpio_read(struct file *file, char *buffer, size_t length, loff_t *offset){

    if (copy_to_user(buffer, &motorInputState, sizeof(motorInputState)) != 0) {
        return -EFAULT;
    }

    return sizeof(motorInputState); // Number of bytes read
}

// prototype for the custom IRQ handler function, function below 
static irq_handler_t  erpi_gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);

static int __init erpi_gpio_init(void) 
{
    majorNumber = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &fops);
    if (majorNumber<0){
        printk(KERN_ALERT "mydev failed to register a major number\n");
        return majorNumber;
    }

    int result = 0;
    printk(KERN_INFO "MOTOR_DRIVER: Initializing the motor_driver LKM\n");

    // if (!gpio_is_valid(gpioLED)) {
    //     printk(KERN_INFO "MOTOR_DRIVER: invalid LED GPIO\n");
    //     return -ENODEV;
    // }   

    // ledOn = true;

    // gpio_request(gpioLED, "sysfs");          // request LED GPIO
    // gpio_direction_output(gpioLED, ledOn);   // set in output mode and on 
    // gpio_set_value(gpioLED, ledOn);       // not reqd - see line above
    // gpio_export(gpioLED, false);             // appears in /sys/class/gpio
                                             // false prevents in/out change   
    gpio_request(gpioMotorInput, "sysfs");       // set up gpioButton   
    gpio_direction_input(gpioMotorInput);        // set up as input   
    gpio_set_debounce(gpioMotorInput, 200);      // debounce delay of 200ms
    gpio_export(gpioMotorInput, false);          // appears in /sys/class/gpio

    printk(KERN_INFO "MOTOR_DRIVER: input value is currently: %d\n", gpio_get_value(gpioMotorInput));

    irqNumber = gpio_to_irq(gpioMotorInput);     // map GPIO to IRQ number
    printk(KERN_INFO "MOTOR_DRIVER: input mapped to IRQ: %d\n", irqNumber);

    // This next call requests an interrupt line   
    result = request_irq(irqNumber,          // interrupt number requested            
        (irq_handler_t) erpi_gpio_irq_handler,   // handler function            
        IRQF_TRIGGER_RISING,                     // on rising edge           
        "erpi_gpio_handler",                     // used in /proc/interrupts
        NULL);                                   // *dev_id for shared interrupt lines
    printk(KERN_INFO "MOTOR_DRIVER: IRQ request result is: %d\n", result);
    return result;
}

static void __exit erpi_gpio_exit(void) 
{   
    // gpio_set_value(gpioLED, 0);              // turn the LED off
    // gpio_unexport(gpioLED);                  // unexport the LED GPIO   
    free_irq(irqNumber, NULL);               // free the IRQ number, no *dev_id   
    gpio_unexport(gpioMotorInput);               // unexport the motor GPIO   
    // gpio_free(gpioLED);                      // free the LED GPIO
    gpio_free(gpioMotorInput);                   // free the motor GPIO
    printk(KERN_INFO "MOTOR_DRIVER: Goodbye from the LKM motor driver!\n"); 
}

static irq_handler_t erpi_gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs) 
{   
    // gpio_set_value(gpioLED, ledOn);
    motorInputState++; //Increment value
    // printk(KERN_INFO "Rising edge !! %d\n", motorInputState);
    return (irq_handler_t) IRQ_HANDLED;      // announce IRQ handled 
}

module_init(erpi_gpio_init);
module_exit(erpi_gpio_exit);