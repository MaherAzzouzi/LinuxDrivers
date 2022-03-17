#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Maher Azzouzi");
MODULE_DESCRIPTION("The Kernel Year");

static int __init hello_init(void)
{
    printk(KERN_ALERT "Hello, world 2022!\n");
    //printk(KERN_ALERT "Kernel Tree version %s\n", UTS_RELEASE);
    printk(KERN_ALERT "Linux Version Code %x\n", LINUX_VERSION_CODE);
    printk(KERN_ALERT "Custom Kernel Version : %x\n"
            , KERNEL_VERSION(5, 5, 10));
    return 0;
}

void function_to_export(void)
{
    return;
}

EXPORT_SYMBOL(function_to_export);

static void __exit hello_exit(void)
{
    printk(KERN_ALERT "Goodbye! 2022\n");
}

module_init(hello_init);
module_exit(hello_exit);
