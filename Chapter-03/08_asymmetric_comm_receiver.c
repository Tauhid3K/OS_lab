/*
 * Section 3.5.2: Direct Communication (Asymmetric Naming)
 * Operating System Concepts, 10th Ed.
 *
 * File 2 of 2: Receiver Process
 * Receives from ANY sender without pre-naming: receive(&id, message)
 * Dynamically identifies the sender PID upon message arrival.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME "/os_asymmetric_queue"

typedef struct {
    pid_t sender_pid;
    char text[128];
} asym_msg_t;

int main(void)
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = sizeof(asym_msg_t);
    attr.mq_curmsgs = 0;

    mq_unlink(QUEUE_NAME); // Clean previous queue if any

    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open (receiver) failed");
        return 1;
    }

    printf("===================================================\n");
    printf(" Asymmetric Communication Receiver [PID: %d]\n", getpid());
    printf(" Waiting for messages from ANY sender process...\n");
    printf(" (Run ./08_asymmetric_comm_sender to send a message)\n");
    printf("===================================================\n\n");

    asym_msg_t msg;
    unsigned int prio;

    // receive(&id, message): does not specify who sends it
    ssize_t bytes = mq_receive(mq, (char *)&msg, sizeof(asym_msg_t), &prio);
    if (bytes == -1) {
        perror("mq_receive failed");
    } else {
        pid_t sender_id = msg.sender_pid; // Dynamically identified!
        printf("[Receiver PID %d] Message Received!\n", getpid());
        printf("  Identified Sender PID (&id): %d\n", sender_id);
        printf("  Message Content:             \"%s\"\n", msg.text);
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    printf("\nAsymmetric communication complete. Queue unlinked.\n");
    return 0;
}
