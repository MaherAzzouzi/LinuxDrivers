#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>

static int __init start_module(void)
{
    dev_t dev;
    int ret;

    printk(KERN_ALERT "Module started!\n");

    ret = alloc_chrdev_region(&dev, 0, 1, "maher");
    if (ret != 0)
    {
        printk(KERN_ALERT "alloc_chrdev_region error!\n");
        return -1;
    }
    return 0;
}

static void __exit finish_module(void)
{
    printk(KERN_ALERT "Module exit!\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maher Azzouzi");

module_init(start_module);
module_exit(finish_module);