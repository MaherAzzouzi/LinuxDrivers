#include <linux/init.h>
#include <linux/module.h>

// For permissions (S_IRUGO)
#include <linux/stat.h>

// To set some already initialized global variables from command line
#include <linux/moduleparam.h>

static char *who_to_greet = "Maher";
static int how_many = 1;
static int greet = 1;
static int nums[3] = {1, 2, 3};

module_param(how_many, int, S_IRUGO);
module_param(who_to_greet, charp, S_IRUGO);
module_param(greet, int, S_IRUGO);
module_param_array(nums, int, 3, S_IRUGO | S_IWUSR);

// The third parameter to module_param is he perm.
// it controls who can access the representation of the module
// parameter in sysfs.
// if it's set to 0, there no sysfs entry at all.
// If it's not 0 it will appear under /sys/module with the given set
// of permissions.
//
// S_IRUGO : can be read by everyone but can not be changed.
// S_IRUGO | S_IWUSR : allow root to change the parameter

static int __init hello_init(void)
{
    int i;

    if (greet)
    {
        for (i = 0; i < how_many; i++)
        {
            printk(KERN_ALERT "Hello , %s!\n", who_to_greet);
        }

        for (i = 0; i < 3; i++)
        {
            printk(KERN_ALERT "NUM[%d] = %d\n", i, nums[i]);
        }
    }
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_ALERT "Good bye, little friend!\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maher Azzouzi");

module_init(hello_init);
module_exit(hello_exit);
