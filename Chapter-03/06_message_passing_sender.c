/*
 * Section 3.5.2: Message-Passing Systems (Operating System Concepts, 10th Ed.)
 * Real Inter-Process Communication (IPC) using POSIX Message Queues.
 *
 * File 1 of 2: Sender Process (Producer of messages)
 * Sends discrete messages across process boundaries via the Linux kernel.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME  "/os_lab_msg_queue"
#define MAX_SIZE    256
#define MAX_MSGS    10

int main(void)
{
    mqd_t mq;
    struct mq_attr attr;

    // Set message queue attributes
    attr.mq_flags = 0;          // Blocking mode
    attr.mq_maxmsg = MAX_MSGS;  // Maximum number of messages in queue
    attr.mq_msgsize = MAX_SIZE; // Maximum size of each message (bytes)
    attr.mq_curmsgs = 0;

    // Create or open the message queue for writing
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open (sender) failed");
        return 1;
    }

    printf("============================================\n");
    printf(" Sender Process Started [PID: %d]\n", getpid());
    printf(" Target Queue: %s\n", QUEUE_NAME);
    printf("============================================\n\n");

    // Messages with different priorities to demonstrate priority queuing
    const char *messages[] = {
        "Message 1: Standard data packet",
        "Message 2: Status report = Normal",
        "Message 3: HIGH PRIORITY - Alert signal!"
    };
    unsigned int priorities[] = {1, 1, 5}; // Message 3 has higher priority (5)
    int num_messages = sizeof(messages) / sizeof(messages[0]);

    for (int i = 0; i < num_messages; i++) {
        printf("[Sender PID %d] Sending (Priority %u): \"%s\"\n", 
               getpid(), priorities[i], messages[i]);

        if (mq_send(mq, messages[i], strlen(messages[i]) + 1, priorities[i]) == -1) {
            perror("mq_send failed");
            mq_close(mq);
            return 1;
        }
    }

    printf("\nAll messages sent successfully to the queue.\n");
    printf("Now run: ./06_message_passing_receiver to retrieve them.\n");

    // Close descriptor in sender (queue remains in kernel until unlinked by receiver)
    if (mq_close(mq) == -1) {
        perror("mq_close failed");
        return 1;
    }

    return 0;
}
