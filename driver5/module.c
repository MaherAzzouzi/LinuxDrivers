#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
// For kzalloc.
#include <linux/slab.h>

static struct class *mclass;
//static struct cdev *cdev;
static dev_t first;

static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode* inode, struct file *file);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
};

struct driver4_struct
{
    char *data;
    unsigned long size;
    struct cdev cdev;
};

static int device_open(struct inode *inode, struct file *file)
{
    struct driver4_struct *drv4;

    printk(KERN_ALERT "open() called!\n");
    printk(KERN_ALERT "major %d minor %d\n", imajor(inode), iminor(inode));
    drv4 = container_of(inode->i_cdev, struct driver4_struct, cdev);
    file->private_data = drv4;

    if ((file->f_flags & O_ACCMODE) == O_RDONLY)
    {
        printk(KERN_ALERT "Opened for write only.\n");
    }
    else if ((file->f_flags & O_ACCMODE) == O_WRONLY)
    {
        printk(KERN_ALERT "Opened for Writing and Reading.\n");
    }

    return 0;
}

static int device_release(struct inode *inode, struct file* file)
{
    printk(KERN_ALERT "Release is called! freeing resources\n");
    kfree(((struct driver4_struct *)file->private_data)->data);
    kfree(file->private_data);
    file->private_data = NULL;
    return 0;
}

static struct driver4_struct *setup_drv4(char *data)
{
    struct driver4_struct *drv4;
    size_t i;

    i = 0;
    while(data[i])
    {
        i++;
    }
    drv4 = kzalloc(sizeof(struct driver4_struct), GFP_KERNEL);
    drv4->data = kzalloc((i + 1) * sizeof(char), GFP_KERNEL);
    memcpy(drv4->data, data, i);

    return drv4;
}

static int __init start_driver(void)
{
    int ret;
    struct device *mdevice;
    struct driver4_struct *drv4;

    printk(KERN_ALERT "Module started!\n");

    drv4 = setup_drv4("MAHERAZZOUZI");

    ret = alloc_chrdev_region(&first, 0, 1, "driver4");
    if (ret < 0)
    {
        return ret;
    }

    mclass = class_create(THIS_MODULE, "driver4class");
    if (IS_ERR(mclass))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(mclass);
    }

    mdevice = device_create(mclass, NULL, first, NULL, "driver%d", 0);
    if(IS_ERR(mdevice))
    {
        class_destroy(mclass);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(mdevice);
    }

    cdev_init(&drv4->cdev, &fops);
    ret = cdev_add(&drv4->cdev, first, 1);
    if (ret < 0)
    {
        device_destroy(mclass, first);
        class_destroy(mclass);
        unregister_chrdev_region(first, 1);
        return ret;
    }

    return 0;
}

static void __exit end_driver(void)
{
    printk(KERN_ALERT "Module exit!\n");
}

module_init(start_driver);
module_exit(end_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maher Azzouzi");