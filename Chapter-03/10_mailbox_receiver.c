/*
 * Section 3.5.2: Mailbox Operations (Lifecycle)
 * Operating System Concepts, 10th Ed.
 *
 * File 2 of 2: Mailbox Manager & Receiver
 * Manages the 3 lifecycle operations:
 *   1. Create mailbox
 *   2. Receive messages from mailbox
 *   3. Destroy (delete/unlink) mailbox
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAILBOX_NAME "/os_lifecycle_queue"
#define MAX_SIZE     256

int main(void)
{
    printf("====================================================\n");
    printf(" Mailbox Lifecycle Manager & Receiver [PID: %d]\n", getpid());
    printf("====================================================\n\n");

    // --------------------------------------------------
    // 1. CREATE MAILBOX
    // --------------------------------------------------
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq_unlink(MAILBOX_NAME); // Clean previous queue if any

    mqd_t mb = mq_open(MAILBOX_NAME, O_CREAT | O_EXCL | O_RDONLY, 0666, &attr);
    if (mb == (mqd_t)-1) {
        perror("[Step 1] Failed to create mailbox");
        return 1;
    }
    printf("[Lifecycle Step 1] CREATED: Mailbox '%s' established in kernel.\n", MAILBOX_NAME);
    printf("Waiting for sender... (Run ./10_mailbox_sender in another shell)\n\n");

    // --------------------------------------------------
    // 2. RECEIVE MESSAGE
    // --------------------------------------------------
    char buffer[MAX_SIZE];
    unsigned int prio;

    ssize_t bytes = mq_receive(mb, buffer, MAX_SIZE, &prio);
    if (bytes >= 0) {
        printf("[Lifecycle Step 2] RECEIVED: \"%s\" (%zd bytes)\n\n", buffer, bytes);
    } else {
        perror("[Step 2] mq_receive failed");
    }

    // --------------------------------------------------
    // 3. DELETE MAILBOX
    // --------------------------------------------------
    mq_close(mb);
    if (mq_unlink(MAILBOX_NAME) == -1) {
        perror("[Step 3] Failed to unlink mailbox");
        return 1;
    }
    printf("[Lifecycle Step 3] DELETED: Mailbox '%s' destroyed from kernel.\n", MAILBOX_NAME);

    printf("\nMailbox lifecycle complete.\n");
    return 0;
}
