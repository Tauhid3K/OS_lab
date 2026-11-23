#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>

/*
 * Kernel Data Structures: Collatz Kernel Module with module_param
 * (Operating System Concepts, 10th Ed., p. 214)
 *
 * Takes a starting integer via module parameter (e.g. insmod kernel_collatz.ko start_n=25),
 * generates Collatz sequence, stores each element in a struct list_head kernel linked list,
 * traverses list to log output, and frees all memory in exit handler.
 */

static int start_n = 25;
module_param(start_n, int, 0644);
MODULE_PARM_DESC(start_n, "Starting integer for the Collatz sequence");

struct collatz_node {
    int value;
    struct list_head list;
};

static LIST_HEAD(collatz_list);

static int __init collatz_kernel_init(void)
{
    struct collatz_node *node, *ptr;
    int n = start_n;

    printk(KERN_INFO "--- Loading Collatz Kernel Module (start_n = %d) ---\n", start_n);

    if (start_n <= 0) {
        printk(KERN_ALERT "Error: start_n must be a positive integer!\n");
        return -EINVAL;
    }

    // Insert initial value
    node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (!node) return -ENOMEM;
    node->value = n;
    INIT_LIST_HEAD(&node->list);
    list_add_tail(&node->list, &collatz_list);

    // Generate sequence
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }

        node = kmalloc(sizeof(*node), GFP_KERNEL);
        if (!node) break;
        node->value = n;
        INIT_LIST_HEAD(&node->list);
        list_add_tail(&node->list, &collatz_list);
    }

    // Traverse and print list
    printk(KERN_INFO "Collatz Sequence generated:\n");
    list_for_each_entry(ptr, &collatz_list, list) {
        printk(KERN_INFO "%d\n", ptr->value);
    }

    return 0;
}

static void __exit collatz_kernel_exit(void)
{
    struct collatz_node *ptr, *next;

    printk(KERN_INFO "--- Removing Collatz Kernel Module and freeing memory ---\n");

    list_for_each_entry_safe(ptr, next, &collatz_list, list) {
        list_del(&ptr->list);
        kfree(ptr);
    }
}

module_init(collatz_kernel_init);
module_exit(collatz_kernel_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Collatz Sequence Kernel Module");
MODULE_AUTHOR("OS Concepts Student");
