/*
 * Section 3.5.2: Indirect Communication (Mailboxes / Ports)
 * Operating System Concepts, 10th Ed.
 *
 * File 1 of 2: Sender Process
 * Deposits messages into a shared mailbox: send(A, message)
 * Does NOT know or reference the receiver's PID.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAILBOX_NAME "/os_mailbox_A"
#define MAX_SIZE     256

int main(void)
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Open or create the mailbox
    mqd_t mb = mq_open(MAILBOX_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    if (mb == (mqd_t)-1) {
        perror("mq_open (sender) failed");
        return 1;
    }

    const char *msg = "Message #300 deposited into Mailbox A";

    printf("===================================================\n");
    printf(" Indirect Communication: Sender Process [PID: %d]\n", getpid());
    printf(" Target: Mailbox '%s' (Receiver PID is unknown)\n", MAILBOX_NAME);
    printf("===================================================\n\n");

    printf("[Sender PID %d] Calling send(%s, \"%s\")...\n", getpid(), MAILBOX_NAME, msg);

    if (mq_send(mb, msg, strlen(msg) + 1, 1) == -1) {
        perror("mq_send failed");
        mq_close(mb);
        return 1;
    }

    printf("[Sender PID %d] Message successfully placed in Mailbox A.\n", getpid());
    printf("Now run ./09_indirect_comm_receiver to retrieve it.\n");

    mq_close(mb);
    return 0;
}
