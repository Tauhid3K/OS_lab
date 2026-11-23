/*
 * Section 3.5.2: Indirect Communication (Mailboxes / Ports)
 * Operating System Concepts, 10th Ed.
 *
 * File 2 of 2: Receiver Process
 * Retrieves messages from the shared mailbox: receive(A, message)
 * Does NOT know or reference the sender's PID.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAILBOX_NAME "/os_mailbox_A"
#define MAX_SIZE     256

int main(void)
{
    mqd_t mb = mq_open(MAILBOX_NAME, O_RDONLY);
    if (mb == (mqd_t)-1) {
        perror("mq_open (receiver) failed");
        fprintf(stderr, "Hint: Run ./09_indirect_comm_sender first to create Mailbox A and deposit a message!\n");
        return 1;
    }

    printf("===================================================\n");
    printf(" Indirect Communication: Receiver Process [PID: %d]\n", getpid());
    printf(" Target: Mailbox '%s' (Sender PID is unknown)\n", MAILBOX_NAME);
    printf("===================================================\n\n");

    char buffer[MAX_SIZE];
    unsigned int prio;

    printf("[Receiver PID %d] Calling receive(%s, message)...\n", getpid(), MAILBOX_NAME);
    ssize_t bytes = mq_receive(mb, buffer, MAX_SIZE, &prio);

    if (bytes >= 0) {
        printf("[Receiver PID %d] Retrieved from Mailbox '%s': \"%s\"\n", getpid(), MAILBOX_NAME, buffer);
    } else {
        perror("mq_receive failed");
    }

    mq_close(mb);
    mq_unlink(MAILBOX_NAME);
    printf("\nMailbox '%s' unlinked and cleaned up.\n", MAILBOX_NAME);
    return 0;
}
