#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "hello"

static ssize_t proc_read(struct file *file, char __user *user_buffer,
                         size_t count, loff_t *position)
{
    char message[] = "Hello World\n";

    return simple_read_from_buffer(user_buffer, count, position,
                                   message, sizeof(message) - 1);
}

static const struct proc_ops proc_file_ops = {
    .proc_read = proc_read
};

static int __init hello_init(void)
{
    proc_create(PROC_NAME, 0444, NULL, &proc_file_ops);

    printk(KERN_INFO "Hello module loaded\n");

    return 0;
}

static void __exit hello_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);

    printk(KERN_INFO "Hello module removed\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");