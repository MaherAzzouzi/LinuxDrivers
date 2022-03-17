#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/stat.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

// operations on proc_dir_entry is not working properly when including <linux/proc_fs.h>
// I included it by path here.
#include "/home/maher/kernel/buildroot/output/build/linux-5.15/fs/proc/internal.h"
//#include <linux/proc_fs.h>
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


static void *device_start(struct seq_file* s, loff_t *pos)
{
    loff_t *spos;

    spos = kmalloc(sizeof(*pos),GFP_KERNEL);
    if (!spos)
        return NULL;
    *spos = *pos;
    MYDEBUG("Start is called!\n");
    seq_printf(s, "device start!\n");
    return spos;
}

static void *device_next(struct seq_file* s, void* v, loff_t *pos)
{
    loff_t *spos = v;
    *pos = ++*spos;

    seq_printf(s, "device next!\n");
    return spos;
}

static void device_stop(struct seq_file* s, void *v)
{
    kfree(v);
    seq_printf(s, "device stop!\n");
    return;
}

static int device_show(struct seq_file* s, void *v)
{
    loff_t *spos = v;
    seq_printf(s, "Hello from device_show()! %lld\n", (long long)*spos);
    return 0;
}

// now that we implemented seq_operations we need to connect it to our
// /proc file we need file_operations (same as char drivers)
static struct seq_operations sops = {
    .start = device_start,
    .next = device_next,
    .stop = device_stop,
    .show = device_show
};

static int file_proc_open(struct inode* inode, struct file* filp)
{
    return seq_open(filp, &sops);
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = file_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release,
};


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

    /*
    pdir = proc_create("maherproc", 0,  NULL, &proc_ops);
    if (IS_ERR(pdir))
    {
        MYDEBUG("Failed to create /proc entry!\n");
        return PTR_ERR(pdir);
    }
    */
    pdir = proc_create_seq_private("maherproc", 0, NULL, &sops, 0, NULL);
    if (IS_ERR(pdir))
    {
        MYDEBUG("proc_create_seq_private failed!");
        return PTR_ERR(pdir);
    }
    // For some reason I can't assign it here, I googled for the error.
    // It's just telling me that struct proc_dir_entry is incomplete.

    // pdir->proc_dir_ops = &fops;

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