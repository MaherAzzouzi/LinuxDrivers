// This module will feature the addition of CAPABILITIES.
#include <linux/sched.h>

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static dev_t first;
static struct class *cls;
static struct device *device;
static struct cdev *cdev;

#define FLAG 'M'
#define CMD1	_IO(FLAG, 0)

static long device_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
{
	if (CMD1 == cmd)
	{
		if (capable(CAP_SYS_ADMIN))
		{
			printk(KERN_ALERT "Oh you have sys admin cap? that's impressing..\n");
			return 0;
		} else {
			printk(KERN_ALERT "Fuck off\n");
			return -EPERM;
		}
	}
	return -EAGAIN;
}

ssize_t device_read(struct file* filp, char __user *buf, size_t count, 
loff_t *fpos)
{
	if (capable(CAP_DAC_OVERRIDE))
	{
		printk(KERN_ALERT "Oh you can override access restrictions (DAC)\n");
	} else {
		printk(KERN_ALERT "You're fucked you have no CAP_DAC_OVERRIFE");
	}
	return 0;
}

static struct file_operations fops = {
	.unlocked_ioctl = device_ioctl,
	.read = device_read,
};

int __init m_start(void)
{
	int ret = alloc_chrdev_region(&first, 1, 0, "module1_cap");
	if (ret < 0)
	{
		return ret;
	}
	cls = class_create(THIS_MODULE, "module1_cap");
	if (IS_ERR(device))
	{
		unregister_chrdev_region(first, 1);
		return PTR_ERR(device);
	}
	device = device_create(cls, NULL, first, NULL, "module1_cap");
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

static void __exit m_exit(void)
{
	cdev_del(cdev);
	device_destroy(cls, first);
	class_destroy(cls);
	unregister_chrdev_region(first, 1);
	return;
}

module_init(m_start);
module_exit(m_exit);

MODULE_AUTHOR("Maher Azzouzi");
MODULE_DESCRIPTION("The shittiest module ever made.");
MODULE_LICENSE("GPL");