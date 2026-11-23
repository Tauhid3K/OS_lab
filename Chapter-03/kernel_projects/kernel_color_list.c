#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/string.h>

/*
 * Kernel Data Structures: Color Linked List
 * (Operating System Concepts, 10th Ed., pp. 212–214)
 *
 * Demonstrates the Linux kernel's circular, doubly linked list struct list_head:
 * - INIT_LIST_HEAD()
 * - list_add_tail()
 * - list_for_each_entry()
 * - list_del()
 * - kmalloc() / kfree()
 */

struct color {
    int red;
    int green;
    int blue;
    char name[16];
    struct list_head list;
};

static LIST_HEAD(color_list);

static void add_color(const char *name, int r, int g, int b)
{
    struct color *node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (!node) return;

    node->red = r;
    node->green = g;
    node->blue = b;
    strscpy(node->name, name, sizeof(node->name));

    INIT_LIST_HEAD(&node->list);
    list_add_tail(&node->list, &color_list);
}

static int __init color_list_init(void)
{
    struct color *ptr;

    printk(KERN_INFO "--- Loading Color List Kernel Module ---\n");

    // Add four colors
    add_color("Red", 255, 0, 0);
    add_color("Green", 0, 255, 0);
    add_color("Blue", 0, 0, 255);
    add_color("Yellow", 255, 255, 0);

    // Traverse and print list
    list_for_each_entry(ptr, &color_list, list) {
        printk(KERN_INFO "Color: %-8s (RGB: %d, %d, %d)\n",
               ptr->name, ptr->red, ptr->green, ptr->blue);
    }

    return 0;
}

static void __exit color_list_exit(void)
{
    struct color *ptr, *next;

    printk(KERN_INFO "--- Removing Color List Kernel Module and freeing memory ---\n");

    list_for_each_entry_safe(ptr, next, &color_list, list) {
        printk(KERN_INFO "Freeing color: %s\n", ptr->name);
        list_del(&ptr->list);
        kfree(ptr);
    }
}

module_init(color_list_init);
module_exit(color_list_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linux Kernel struct list_head Demo");
MODULE_AUTHOR("OS Concepts Student");
