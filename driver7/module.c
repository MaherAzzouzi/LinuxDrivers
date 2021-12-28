#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/stat.h>
#include <linux/proc_fs.h>

#define DEBUG_MODE 1

#undef MYDEBUG
#ifdef DEBUG_MODE
#   ifdef __KERNEL__
#       define MYDEBUG(fmt, args...) printk(KERN_ALERT "module : " fmt, ## args)
#   else
#       define MYDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#   endif
#else
#   define MYDEBUG(fmt, args...)
#endif

dev_t first;
struct class *cls;
struct device *device;

static int module_open(struct inode* inode, struct file* filp)
{
    MYDEBUG("module_open() called!\n");
    return 0;
}

static ssize_t module_read(struct file* filp, char __user *buf, size_t count , loff_t *fpos)
{
    MYDEBUG("module_read() called!\n");
    return 0;
}

static ssize_t module_write(struct file* filp, const char __user *buf, size_t count, loff_t *fpos)
{
    MYDEBUG("module_write() called!\n");
    return count;
}

static loff_t module_lseek(struct file* file, loff_t fpos, int count)
{
    MYDEBUG("module_lseek() called!\n");
    return count;
}

static struct proc_ops proc_ops =
{
    .proc_open = module_open,
    .proc_read = module_read,
    .proc_write = module_write,
    .proc_lseek = module_lseek,
};

static int __init debugm_start(void)
{
    int i;
    int ret;
    char buf[100];

    printk(KERN_ALERT "Module started!\n");

    MYDEBUG("If you see this then MYDEBUG is enabled!\n");

    for(i = 0; i < 0x10; i++)
    {
        if (printk_ratelimit())
        {
            MYDEBUG("printk_ratelimit starts returning 0 when the level of output exceeds a threshold\n");
            MYDEBUG("/proc/sys/kernel/printk_ratelimit_burst is the threshold file (10 by default)\n");
            MYDEBUG("/proc/sys/kernel/printk_ratelimit the number of seconds to wait before calculating again (5 by default)\n");
        }
    }

    ret = alloc_chrdev_region(&first, 0, 1, "MAHER");
    if (ret < 0)
    {
        MYDEBUG("alloc_chrdev_region problem\n");
        return ret;
    }

    print_dev_t(buf, first);
    MYDEBUG("MAJOR:MIN %s", buf);

    cls = class_create(THIS_MODULE, "maher");
    if (IS_ERR(cls))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cls);
    }

    device = device_create(cls, NULL, first, NULL, "driver7");
    if (IS_ERR(device))
    {
        class_destroy(cls);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cls);
    }

    struct proc_dir_entry *pdir;

    pdir = proc_mkdir("maherdir", NULL);
    if (IS_ERR(pdir))
    {
        MYDEBUG("Failed to create the repo inside proc\n");
        return PTR_ERR(pdir);
    }

    pdir = proc_create("maherproc", 0660,  pdir, &proc_ops);
    if (IS_ERR(pdir))
    {
        MYDEBUG("Failed to create /proc entry!\n");
        return PTR_ERR(pdir);
    }
    
    pdir = proc_create("maherproc0", 0660, pdir, &proc_ops);
    if (IS_ERR(pdir))
    {
        MYDEBUG("Failed to create subfolder inside /proc!\n");
        return PTR_ERR(pdir);
    }

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