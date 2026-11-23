/*
 * Section 3.5.2 & Figure 3.14: Producer using Message Passing
 * Operating System Concepts, 10th Ed.
 *
 * File 1 of 2: Producer Process
 * Produces items and sends them through the message queue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME  "/os_prod_cons_mqueue"
#define MAX_ITEMS   5
#define STOP_SIGNAL -1

typedef struct {
    int item_id;
    int data;
} item_t;

int main(void)
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(item_t);
    attr.mq_curmsgs = 0;

    // Create or open the message queue
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open (producer) failed");
        return 1;
    }

    printf("===================================================\n");
    printf(" Producer Process Started [PID: %d]\n", getpid());
    printf(" Producing %d items into queue '%s'\n", MAX_ITEMS, QUEUE_NAME);
    printf("===================================================\n\n");

    for (int i = 1; i <= MAX_ITEMS; i++) {
        item_t item;
        item.item_id = i;
        item.data = i * 10;

        printf("[Producer PID %d] Produced Item #%d (Data = %d) -> send()\n",
               getpid(), item.item_id, item.data);

        if (mq_send(mq, (const char *)&item, sizeof(item_t), 1) == -1) {
            perror("mq_send failed");
            break;
        }
        usleep(150000); // 150ms simulation delay
    }

    // Send STOP sentinel
    item_t stop = {.item_id = STOP_SIGNAL, .data = 0};
    mq_send(mq, (const char *)&stop, sizeof(item_t), 1);
    printf("\n[Producer PID %d] Sent STOP signal to Consumer.\n", getpid());
    printf("Now run ./11_consumer_msg_passing to consume all items.\n");

    mq_close(mq);
    return 0;
}
