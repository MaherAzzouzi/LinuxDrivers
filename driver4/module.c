#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

static dev_t first;
static struct cdev *cdev;
static struct class *null_class;

static void __exit null_end(void)
{
    cdev_del(cdev);
    device_destroy(null_class, first);
    class_destroy(null_class);
    unregister_chrdev_region(first, 1);
}

static int null_open(struct inode *i, struct file *filp)
{
    printk(KERN_ALERT "open()\n");
    return 0;
}

static int null_release(struct inode *i, struct file *filp)
{
    printk(KERN_ALERT "release()\n");
    return 0;
}

static ssize_t null_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_ALERT "read()\n");
    return 0;
}

static ssize_t null_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_ALERT "write()\n");
    return len;
}

static struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .read = null_read,
    .write = null_write,
    .open = null_open,
    .release = null_release,
};

/*
static int __init null_start(void)
{
    int ret;
    struct device *null_device;

    printk(KERN_ALERT "Null started!");

    ret = alloc_chrdev_region(&first, 0, 1, "null_chrdevregion");
    if (ret < 0)
    {
        return ret;
    }
    null_class = class_create(THIS_MODULE, "nullclass");
    if (IS_ERR(null_class))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(null_class);
    }

    if (IS_ERR(null_device = device_create(null_class, NULL, first, NULL, "mynull")))
    {
        class_destroy(null_class);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(null_device);
    }

    cdev = cdev_alloc();
    cdev->ops = &fops;
    cdev->owner = THIS_MODULE;
    ret = cdev_add(cdev, first, 1);
    if (ret < 0)
    {
        device_destroy(null_class, first);
        class_destroy(null_class);
        unregister_chrdev_region(first, 1);
        return ret;
    }
    printk(KERN_ALERT "mynull loaded!\n");
    return 0;
}
*/

static int __init null_start(void)
{
    struct device *null_device;

    alloc_chrdev_region(&first, 0, 1, "nullregion");

    null_class = class_create(THIS_MODULE, "nullclass");
    if (IS_ERR(null_class))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(null_class);
    }

    null_device = device_create(null_class, NULL, first, NULL, "mynull%d", 0);
    if (IS_ERR(null_device))
    {
        class_destroy(null_class);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(null_device);
    }

    
    cdev = cdev_alloc();
    cdev->ops = &fops;
    cdev->owner = THIS_MODULE;
    if (cdev_add(cdev, first, 1) < 0)
    {
        device_destroy(null_class, first);
        class_destroy(null_class);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    return 0;
}

module_init(null_start);
module_exit(null_end);

MODULE_AUTHOR("Maher Azzouzi");
MODULE_LICENSE("GPL");