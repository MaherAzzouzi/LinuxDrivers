#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#define DEBUG_MODE 1

#undef MYDEBUG
#ifdef DEBUG_MODE
#   ifdef __KERNEL__
#       define MYDEBUG(fmt, args...) printk(KERN_DEBUG "module : " fmt, ## args)
#   else
#       define MYDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#   endif
#else
#   define MYDEBUG(fmt, args...)
#endif

static int __init debugm_start(void)
{
    printk(KERN_ALERT "Module started!\n");

    MYDEBUG("If you see this then MYDEBUG is enabled!\n");

    return 0;
}

static void __exit debugm_exit(void)
{
    printk(KERN_DEBUG "Module exit!\n");
}

module_init(debugm_start);
module_exit(debugm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maher Azzouzi");