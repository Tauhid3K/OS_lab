#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>

#define PROC_NAME "jiffies"

static ssize_t proc_read(struct file *file, char __user *user_buffer,
                         size_t count, loff_t *position)
{
    char buffer[100];

    int length = snprintf(buffer, sizeof(buffer),
                          "%lu\n", jiffies);

    return simple_read_from_buffer(user_buffer, count, position,
                                   buffer, length);
}

static const struct proc_ops proc_file_ops = {
    .proc_read = proc_read
};

static int __init jiffies_init(void)
{
    proc_create(PROC_NAME, 0444, NULL, &proc_file_ops);

    printk(KERN_INFO "Jiffies module loaded\n");

    return 0;
}

static void __exit jiffies_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);

    printk(KERN_INFO "Jiffies module removed\n");
}

module_init(jiffies_init);
module_exit(jiffies_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");