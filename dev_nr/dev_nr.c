#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ram Jayakumar GNU/Linux");
MODULE_DESCRIPTION("Registers a device nr. with some callback functions");

/**
 * @brief This function is called when the device file is opened
 */
static int driver_open(struct inode* device_file, struct file* instance) {
	printk("dev_nr - open was called!\r\n");
	return 0;
}

/**
 * @brief This function is called when the device file is closed
 */
static int driver_close(struct inode* device_file, struct file* instance) {
	printk("dev_nr - close was called!\r\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close
};

#define MYMAJOR 90

/**
 * @brief This function is called when the module is loaded into the kernel
 */

static int __init ModuleInit(void) {
	int retval;
	printk("Hello Kernel!\r\n");
	/* register device nr. */
	retval = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
	if (retval == 0) {
		printk("dev_nr - registered Device number Major: %d, Minor: %d", MYMAJOR, 0);
	} else if (retval > 0) {
		printk("dev_nr - registered Device number Major: %d, Minor: %d", retval >> 20, 0xfffff & retval);
	} else {
		printk("dev_nr - Could not register device number!\r\n"); 
		return -1;
	}
	return 0;
}

/**
 * @brief This function is called when the module is removed from the kernel
 */

static void __exit ModuleExit(void) {
	unregister_chrdev(MYMAJOR, "my_dev_nr");
	printk("Goodbye Kernel!\r\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
		
