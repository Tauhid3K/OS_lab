/*
 * Section 3.5.2 & Figure 3.15: Consumer using Message Passing
 * Operating System Concepts, 10th Ed.
 *
 * File 2 of 2: Consumer Process
 * Repeatedly receives and consumes items until stop signal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME  "/os_prod_cons_mqueue"
#define STOP_SIGNAL -1

typedef struct {
    int item_id;
    int data;
} item_t;

int main(void)
{
    mqd_t mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open (consumer) failed");
        fprintf(stderr, "Hint: Run ./11_producer_msg_passing first to produce items into the queue!\n");
        return 1;
    }

    printf("===================================================\n");
    printf(" Consumer Process Started [PID: %d]\n", getpid());
    printf(" Consuming items from queue '%s'\n", QUEUE_NAME);
    printf("===================================================\n\n");

    item_t item;
    unsigned int prio;
    int count = 0;

    while (1) {
        ssize_t bytes = mq_receive(mq, (char *)&item, sizeof(item_t), &prio);
        if (bytes == -1) {
            perror("mq_receive failed");
            break;
        }

        if (item.item_id == STOP_SIGNAL) {
            printf("\n[Consumer PID %d] Received STOP signal. All items consumed.\n", getpid());
            break;
        }

        count++;
        printf("  [Consumer PID %d] Consumed Item #%d: Value = %d\n",
               getpid(), item.item_id, item.data);
    }

    printf("\nTotal items consumed: %d\n", count);

    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    printf("Queue '%s' closed and unlinked successfully.\n", QUEUE_NAME);
    return 0;
}
