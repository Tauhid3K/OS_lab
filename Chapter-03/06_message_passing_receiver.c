/*
 * Section 3.5.2: Message-Passing Systems (Operating System Concepts, 10th Ed.)
 * Real Inter-Process Communication (IPC) using POSIX Message Queues.
 *
 * File 2 of 2: Receiver Process (Consumer of messages)
 * Retrieves discrete messages sent by the Sender Process from the kernel queue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME  "/os_lab_msg_queue"
#define MAX_SIZE    256

int main(void)
{
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE];
    unsigned int priority;

    // Open existing message queue for reading
    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open (receiver) failed");
        fprintf(stderr, "\nHint: Run ./06_message_passing_sender first to create the queue and send messages!\n");
        return 1;
    }

    // Query queue attributes to check current messages
    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr failed");
        mq_close(mq);
        return 1;
    }

    printf("============================================\n");
    printf(" Receiver Process Started [PID: %d]\n", getpid());
    printf(" Connected to Queue: %s\n", QUEUE_NAME);
    printf(" Messages currently in queue: %ld\n", attr.mq_curmsgs);
    printf("============================================\n\n");

    // Retrieve messages (POSIX message queues return higher priority first!)
    int count = 0;
    while (attr.mq_curmsgs > 0) {
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, &priority);
        if (bytes_read == -1) {
            perror("mq_receive failed");
            break;
        }

        count++;
        printf("[Receiver PID %d] Received #%d (Priority %u, %zd bytes): \"%s\"\n",
               getpid(), count, priority, bytes_read, buffer);

        // Update remaining count
        if (mq_getattr(mq, &attr) == -1) {
            break;
        }
    }

    printf("\nFinished receiving %d messages.\n", count);

    // Close the message queue descriptor
    if (mq_close(mq) == -1) {
        perror("mq_close failed");
        return 1;
    }

    // Remove the message queue from the operating system
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink failed");
        return 1;
    }

    printf("Queue %s closed and unlinked successfully.\n", QUEUE_NAME);
    return 0;
}
