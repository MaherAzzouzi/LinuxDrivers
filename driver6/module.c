#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uio.h>

static dev_t first;
static struct cdev *cdev;
static struct class *cls;

static ssize_t driver6_read(struct file* filp, char __user *buf, size_t count, loff_t *ppos);
static ssize_t driver6_write(struct file* filp, const char __user *buf, size_t count, loff_t *ppos);

// it turns out that there is no readv and writev in newer kernels.
// shit! things got complicated!
static ssize_t driver6_writev(struct file* filp, const struct iovec *iov, unsigned long count, loff_t *ppos)
{
    int i;

    for(i = 0; i < count ; i++)
    {
        driver6_write(filp, iov[i].iov_base, iov[i].iov_len, ppos);
    }
    return 0;
}

static ssize_t driver6_readv(struct file* filp, const struct iovec *iov, unsigned long count, loff_t *ppos)
{
    int i;

    for(i = 0; i < count; i++)
    {
        driver6_read(filp, iov[i].iov_base, iov[i].iov_len, ppos);
    }
    return 0;
}

static ssize_t driver6_read(struct file* filp, char __user *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_ALERT "read() is called!\n");
    return 0;
}

static ssize_t driver6_write(struct file* filp, const char __user *buf, size_t count, loff_t *fpos)
{
    printk(KERN_ALERT "write() is called!\n");
    return count;
}

static struct file_operations fops = 
{
    .read = driver6_read,
    .write = driver6_write,

};

static int __init driver_initialization(void)
{
    static struct device* device6;
    int ret;

    ret = alloc_chrdev_region(&first, 0, 1, "driver6");
    if (ret < 0)
    {
        return ret;
    }
    cls = class_create(THIS_MODULE, "driver6class");
    if (IS_ERR(cls))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cls);
    }

    device6 = device_create(cls, NULL, first, NULL, "driver%d", 6);
    if (IS_ERR(device6))
    {
        class_destroy(cls);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(device6);
    }

    cdev = cdev_alloc();
    cdev_add(cdev, first, 1);
    cdev->ops = &fops;

    return 0;
}

static void __exit driver_finish(void)
{

}

module_init(driver_initialization);
module_exit(driver_finish);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maher Azzouzi");