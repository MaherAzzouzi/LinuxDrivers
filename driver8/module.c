#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/cdev.h>
// The purpose for this module is to introduce Mutexes
// To avoid race conditions.
#include <linux/semaphore.h>

// For read/write semaphores
// The idea if two threads both want to access a data structure for reading
// There is no purpose for making one run and the other waiting on the lock
// They only want to read
#include <linux/rwsem.h>

static dev_t first;
static struct class *cls;
static struct device *device;
static struct cdev *cdev;

DEFINE_SEMAPHORE(lock);


// Lock only `critical sections` that will perform read or write operations.
// This have speed benefits.
// downgrade_write() can be used to unlock all other readers to read newly created data.
// It allows one writer or unlimited number of readers to hold the semaphore.
// Writers have prio, as soon as a write tries to enter the critical section no readers
// will be allowed to read until the write is finished.
// (it will be efficient when the number of writes is small than the number of reads)
struct rw_semaphore sem;

int i;

// I will be using read to increment i, and write to show i.
// It should be locked properly so it can not be incremented twice before getting printed.
static ssize_t mymodule_read(struct file* filp, char __user *buf, size_t count, loff_t *fpos)
{
    down_write(&sem);
    // We have the lock we can increment i
    i++;

    // Release the lock here.
    up_write(&sem);
    up(&lock);
    return 0;
}

static ssize_t mymodule_write(struct file* filp, const char __user *buf, size_t count, loff_t *fpos)
{
    if (down_interruptible(&lock))
    {
        return -ERESTARTSYS;
    }

    down_read(&sem);

    printk(KERN_ALERT "The number is %d\n", i);

    up_read(&sem);

    return count;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = mymodule_read,
    .write = mymodule_write,
};


static int __init mymodule_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&first, 0, 1, "driver8");
    if (ret < 0)
    {
        return ret;
    }
    cls = class_create(THIS_MODULE, "driver8");
    if (IS_ERR(cls))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cls);
    }

    device = device_create(cls, NULL, first, NULL, "device8");
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

    init_rwsem(&sem);

    printk(KERN_ALERT "Module loaded successfuly!\n");
    return 0;
}

static void __exit mymodule_exit(void)
{
    cdev_del(cdev);
    device_destroy(cls, first);
    class_destroy(cls);
    unregister_chrdev_region(first, 1);
    printk(KERN_ALERT "Module unloaded successfuly!\n");
    return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maher Azzouzi");