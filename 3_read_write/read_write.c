#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ram Jayakumar GNU/Linux");
MODULE_DESCRIPTION("Registers a device nr. with some callback functions");

static char buffer[255];
static int buffer_pointer;

/**
 * @brief Read data out of buffer
 */
static ssize_t driver_read(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;

	/* Get amount of data to copy */
	to_copy = min(count, buffer_pointer);

	// can't use memcpy to copy from kernel to user space, need to use uaccess
	// dest, source, amount of bytes to copy
	// copy_to_user returns num of bytes NOT copied
	not_copied = copy_to_user(user_buffer, buffer, to_copy);

	/* Calculate data -- number of bytes it has copied
	 * ie if tried to copy 6 bytes and not_copied is 4, only 2 bytes copied
	 */
	delta = to_copy - not_copied;
	
	return delta;
}


/**
 * @brief Write data to buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(buffer));

	// can't use memcpy to copy from kernel to user space, need to use uaccess
	// dest, source, amount of bytes to copy
	// copy_to_user returns num of bytes NOT copied
	not_copied = copy_from_user(buffer, user_buffer, to_copy);
	buffer_pointer = to_copy;

	/* Calculate data -- number of bytes it has copied
	 * ie if tried to copy 6 bytes and not_copied is 4, only 2 bytes copied
	 */
	delta = to_copy - not_copied;
	
	return delta;
}

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
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
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
		
