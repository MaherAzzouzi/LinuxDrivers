// There is nothing advanced in this character device.
// All shitty, just need to go in coordination with the book.

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
// This header for those functions to put data into user 1/2/4/8 bytes
#include <asm/uaccess.h>
dev_t first;
static struct class *cls;
static struct device *device;
static struct cdev *cdev;

#define MODULE_IOC_MAGIC 'M'
#define MODULE_READ		_IOR(MODULE_IOC_MAGIC, 0, char *)
#define MODULE_WRITE	_IOW(MODULE_IOC_MAGIC, 1, char *)

// According to POSIX standard -ENOTTY should be returned if the user provided
// an invalid command to ioctl.
// Will complete this one later

// Global buffer to be used for userland to hide things in the kernel.
char buf[0x1000];

static long file_ioctl(struct file* filp, unsigned int cmd,
						unsigned long arg)
{
	// We will copy 0x1000 data from userland no matter what the size of the
	// buffer provided by the user.
	printk(KERN_ALERT "file ioctl called!\n");
	if (cmd == MODULE_READ) {
		if (copy_from_user(buf, (char *)arg, sizeof(buf) - 1))
		{
			// Something went wrong!
			return -EAGAIN;
		}
	}
	else if (cmd == MODULE_WRITE) {
		if (copy_to_user((char *)arg, buf, sizeof(buf) -1))
		{
			// Again something is wrong, copy_from_user & copy_to_user return a 
			// non-zero value in case of failure (the return value is how much 
			// bytes left not sended)
			return -EAGAIN;
		}
	}
	return 0;
}

static struct file_operations fops = 
{
	.unlocked_ioctl = file_ioctl,
};

static int __init module_start(void)
{
	int ret = alloc_chrdev_region(&first, 0, 1, "module0_ioctl");
	if (ret < 0)
	{
		return ret;
	}

	cls = class_create(THIS_MODULE, "module0_ioctl");
	if (IS_ERR(cls))
	{
		unregister_chrdev_region(first, 1);
		return PTR_ERR(cls);
	}
	device = device_create(cls, NULL, first, NULL, "module0_ioctl");
	if (IS_ERR(device))
	{
		class_destroy(cls);
		unregister_chrdev_region(first, 1);
		return PTR_ERR(device);
	}

	cdev = cdev_alloc();

	if (IS_ERR(cdev))
	{
		device_destroy(cls, first);
		class_destroy(cls);
		unregister_chrdev_region(first, 1);
		return PTR_ERR(cdev);
	}
	cdev_add(cdev, first, 1);
	cdev->ops = &fops;
	return 0;
}

static void __exit module_finish(void)
{
	cdev_del(cdev);
	device_destroy(cls, first);
	class_destroy(cls);
	unregister_chrdev_region(first, 1);
	return;
}

module_init(module_start);
module_exit(module_finish);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maher Azzouzi");