#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/pid.h>

/*
 * Programming Project 2: Linux Kernel Module for Task Information (Figure 3.37)
 * (Operating System Concepts, 10th Ed., pp. 207–209)
 *
 * Creates /proc/pid.
 * When written to (e.g., echo "1" > /proc/pid), records the PID.
 * When read (e.g., cat /proc/pid), displays the command, PID, and state of that task.
 */

#define PROC_NAME "pid"
#define BUFFER_SIZE 128

static char proc_buf[BUFFER_SIZE];
static int target_pid = -1;

static ssize_t proc_write(struct file *file, const char __user *user_buffer,
                          size_t count, loff_t *position)
{
    char k_buf[32];
    size_t copy_size = (count < sizeof(k_buf) - 1) ? count : (sizeof(k_buf) - 1);

    if (copy_from_user(k_buf, user_buffer, copy_size)) {
        return -EFAULT;
    }
    k_buf[copy_size] = '\0';

    if (kstrtoint(k_buf, 10, &target_pid) < 0) {
        // Try parsing without trailing newline
        if (sscanf(k_buf, "%d", &target_pid) != 1) {
            return -EINVAL;
        }
    }

    printk(KERN_INFO "pid_info: Target PID set to %d\n", target_pid);
    return count;
}

static ssize_t proc_read(struct file *file, char __user *user_buffer,
                         size_t count, loff_t *position)
{
    struct task_struct *task;
    int len = 0;

    if (target_pid < 0) {
        len = snprintf(proc_buf, sizeof(proc_buf), "No PID specified. Write a PID to /proc/%s first.\n", PROC_NAME);
    } else {
        rcu_read_lock();
        task = pid_task(find_vpid(target_pid), PIDTYPE_PID);
        if (task) {
            len = snprintf(proc_buf, sizeof(proc_buf),
                           "command = [%s], pid = [%d], state = [%u]\n",
                           task->comm, task->pid, (unsigned int)task->__state);
        } else {
            len = snprintf(proc_buf, sizeof(proc_buf), "PID %d not found.\n", target_pid);
        }
        rcu_read_unlock();
    }

    return simple_read_from_buffer(user_buffer, count, position, proc_buf, len);
}

static const struct proc_ops proc_file_ops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

static int __init proc_pid_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_file_ops);
    printk(KERN_INFO "/proc/%s created successfully.\n", PROC_NAME);
    return 0;
}

static void __exit proc_pid_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed.\n", PROC_NAME);
}

module_init(proc_pid_init);
module_exit(proc_pid_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task Information Kernel Module");
MODULE_AUTHOR("OS Concepts Student");
