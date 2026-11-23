/*
 * Section 3.5.2: Direct Communication (Symmetric Naming)
 * Operating System Concepts, 10th Ed.
 *
 * File 2 of 2: Receiver Process
 * Explicitly expects and verifies the recipient identity: receive(Q, message)
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME "/os_direct_queue"
#define PID_FILE   "/tmp/os_direct_receiver.pid"

typedef struct {
    pid_t sender_pid;
    pid_t target_pid;
    char text[128];
} direct_msg_t;

int main(void)
{
    pid_t my_pid = getpid();

    // Publish own PID so sender knows who to target
    FILE *f = fopen(PID_FILE, "w");
    if (f) {
        fprintf(f, "%d\n", my_pid);
        fclose(f);
    }

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = sizeof(direct_msg_t);
    attr.mq_curmsgs = 0;

    mq_unlink(QUEUE_NAME); // Clean previous queue if any

    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open (receiver) failed");
        unlink(PID_FILE);
        return 1;
    }

    printf("===================================================\n");
    printf(" Direct Symmetric Communication: Receiver [PID: %d]\n", my_pid);
    printf(" Waiting for a message specifically addressed to me...\n");
    printf(" (Run ./07_direct_comm_sender in another terminal/shell)\n");
    printf("===================================================\n\n");

    direct_msg_t msg;
    unsigned int prio;

    ssize_t bytes = mq_receive(mq, (char *)&msg, sizeof(direct_msg_t), &prio);
    if (bytes == -1) {
        perror("mq_receive failed");
    } else {
        printf("[Receiver PID %d] Message Arrived!\n", my_pid);
        printf("  Sender PID:   %d\n", msg.sender_pid);
        printf("  Target PID:   %d\n", msg.target_pid);
        printf("  Message Text: \"%s\"\n\n", msg.text);

        // Symmetric verification
        if (msg.target_pid == my_pid) {
            printf("[Receiver PID %d] VERIFIED: Message was explicitly addressed to my PID.\n", my_pid);
        } else {
            printf("[Receiver PID %d] WARNING: Target PID mismatch!\n", my_pid);
        }
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    unlink(PID_FILE);
    printf("Direct communication complete. Queue unlinked.\n");
    return 0;
}
