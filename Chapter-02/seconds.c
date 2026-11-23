#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>

#define PROC_NAME "seconds"

static unsigned long start_time;

static ssize_t proc_read(struct file *file, char __user *user_buffer,
                         size_t count, loff_t *position)
{
    char buffer[100];

    unsigned long elapsed;

    elapsed = (jiffies - start_time) / HZ;

    int length = snprintf(buffer, sizeof(buffer),
                          "%lu\n", elapsed);

    return simple_read_from_buffer(user_buffer, count, position,
                                   buffer, length);
}

static const struct proc_ops proc_file_ops = {
    .proc_read = proc_read
};

static int __init seconds_init(void)
{
    start_time = jiffies;

    proc_create(PROC_NAME, 0444, NULL, &proc_file_ops);

    printk(KERN_INFO "Seconds module loaded\n");

    return 0;
}

static void __exit seconds_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);

    printk(KERN_INFO "Seconds module removed\n");
}

module_init(seconds_init);
module_exit(seconds_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");