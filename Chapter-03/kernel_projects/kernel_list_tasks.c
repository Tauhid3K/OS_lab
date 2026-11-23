#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/list.h>

/*
 * Programming Project 3: Linux Kernel Module for Listing Tasks
 * (Operating System Concepts, 10th Ed., pp. 210–212)
 *
 * Part I: Linear traversal of tasks using for_each_process(task) macro.
 * Part II: Depth-First Search (DFS) tree traversal using children and sibling pointers.
 */

// Recursive Depth-First Search (DFS) task listing
static void dfs_traverse(struct task_struct *task)
{
    struct task_struct *child;
    struct list_head *list;

    printk(KERN_INFO "DFS Task: comm: %-16s | pid: %-6d | state: %u\n",
           task->comm, task->pid, (unsigned int)task->__state);

    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        dfs_traverse(child);
    }
}

static int __init list_tasks_init(void)
{
    struct task_struct *task;

    printk(KERN_INFO "====================================================\n");
    printk(KERN_INFO "  Part I: Linear Process Listing (for_each_process) \n");
    printk(KERN_INFO "====================================================\n");

    rcu_read_lock();
    for_each_process(task) {
        printk(KERN_INFO "Linear Task: comm: %-16s | pid: %-6d | state: %u\n",
               task->comm, task->pid, (unsigned int)task->__state);
    }
    rcu_read_unlock();

    printk(KERN_INFO "\n====================================================\n");
    printk(KERN_INFO "  Part II: Depth-First Search (DFS) Process Tree    \n");
    printk(KERN_INFO "====================================================\n");

    rcu_read_lock();
    dfs_traverse(&init_task);
    rcu_read_unlock();

    return 0;
}

static void __exit list_tasks_exit(void)
{
    printk(KERN_INFO "List Tasks Kernel Module unloaded.\n");
}

module_init(list_tasks_init);
module_exit(list_tasks_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Listing Tasks Linearly and with DFS");
MODULE_AUTHOR("OS Concepts Student");
