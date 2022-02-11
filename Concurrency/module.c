#include <linux/module.h>
#include <linux/fs.h>


#include <linux/device.h>
#include <linux/cdev.h>

// Using these to assert concurrency.
#include <linux/semaphore.h>
#include <linux/completion.h>
#include <linux/spinlock.h>


dev_t first;
struct class *cls;
struct device *device;
struct cdev *cdev;

DEFINE_SEMAPHORE(sem);
DECLARE_COMPLETION(completion);
spinlock_t spinlock;
// I will increment this variable and print it atomically.
// The lock will not be released until it is printed after incrementing.
ssize_t num = 0;

static ssize_t mymodule_read(struct file* filp, char __user *buf, size_t count, loff_t *fpos)
{
    /*
    if (down_killable(&sem))
    {
        return -ERESTARTSYS;
    }
    */
    //wait_for_completion(&completion);
    num++;
    return 0;
}

static ssize_t mymodule_write(struct file* filp, const char __user *buf, size_t count, loff_t *fpos)
{
    printk(KERN_ALERT "The number is %ld\n", num);
    //complete(&completion);
    //up(&sem);
    return count;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = mymodule_read,
    .write = mymodule_write,
};

int __init module_start(void) {
    int ret;

    printk(KERN_ALERT "module started!\n");
    ret = alloc_chrdev_region(&first, 0, 1, "sem");
    if (ret < 0)
        return ret;
    cls = class_create(THIS_MODULE, "sem_class");
    if (IS_ERR(cls)) {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cls);
    }

    device = device_create(cls, NULL, first, NULL, "sem_device");
    if (IS_ERR(device)) {
        class_destroy(cls);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(device);
    }

    cdev = cdev_alloc();
    if (IS_ERR(cdev)) {
        device_destroy(cls, first);
        class_destroy(cls);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cdev);
    }

    cdev_add(cdev, first, 1);
    cdev->ops = &fops;

    spin_lock_init(&spinlock);
    return 0;
}

void __exit module_finish(void)
{
    cdev_del(cdev);
    device_destroy(cls, first);
    class_destroy(cls);
    unregister_chrdev_region(first, 1);
    printk(KERN_ALERT "module exit!\n");
}

module_init(module_start);
module_exit(module_finish);

MODULE_AUTHOR("MAHER AZZOUZI");
MODULE_DESCRIPTION("Using semaphores this time.");
MODULE_LICENSE("GPL");