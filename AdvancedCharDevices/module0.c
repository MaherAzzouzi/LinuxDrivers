// There is nothing advanced in this character device.
// All shitty just to go in coordination with the book.

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

static dev_t first;
static struct class *cls;
static struct driver *drv;
static struct cdev *cdev;

#define MODULE_IOC_MAGIC 'M'

#define MODULE_READ		_IOR(MODULE_IOC_MAGIC, 0, int)
#define MODULE_WRITE	_IOR(MODULE_IOC_MAGIC, 1)

// According to POSIX standard -ENOTTY should be returned if the user provided
// an invalid command to ioctl.
// Will complete this one later

static int __init module_start()
{
	int ret = alloc_chrdev_region(&first, 0, 1, "module0_ioctl");
	if (ret < 0)
	{
		return ret;
	}

	cls = class_create(THIS_MODULE, "module0_ioctl");
	if (IS_ERR(cls))
	{
		unregister_chrdev_region(&first, 1);
		return PTR_ERR(cls);
	}
	device = device_create(cls, NULL, first, NULL, "module0_ioctl");
	if (IS_ERR(device))
	{
		class_destroy(cls);
		unregister_chrdev_region(&first, 1);
		return PTR_ERR(device);
	}

	cdev = cdev_alloc();

	if (IS_ERR(cdev))
	{
		device_destroy(cls, first);
		class_destroy(cls);
		unregister_chrdev_region(&first, 1);
		return PTR_ERR(cdev);
	}
	cdev_add(cdev, first, 1);
	cdev->ops = &fops;
	return 0;
}

static void __exit module_finish()
{
	cdev_del(cdev);
	device_destroy(cls, first);
	class_destroy(cls);
	unregister_chrdev_region(&first, 1);
	return;
}