/*
 * Section 3.5.2: Mailbox Operations (Lifecycle)
 * Operating System Concepts, 10th Ed.
 *
 * File 1 of 2: Sender Process (Deposits message into active mailbox)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAILBOX_NAME "/os_lifecycle_queue"

int main(void)
{
    mqd_t mb = mq_open(MAILBOX_NAME, O_WRONLY);
    if (mb == (mqd_t)-1) {
        perror("mq_open failed");
        fprintf(stderr, "Hint: Run ./10_mailbox_receiver first! The receiver creates the mailbox.\n");
        return 1;
    }

    const char *msg = "Message payload 500 across mailbox";
    printf("[Sender PID %d] Sending message to Mailbox '%s': \"%s\"\n",
           getpid(), MAILBOX_NAME, msg);

    if (mq_send(mb, msg, strlen(msg) + 1, 1) == -1) {
        perror("mq_send failed");
        mq_close(mb);
        return 1;
    }

    printf("[Sender PID %d] Message sent successfully.\n", getpid());
    mq_close(mb);
    return 0;
}
