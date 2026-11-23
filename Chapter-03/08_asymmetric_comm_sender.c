/*
 * Section 3.5.2: Direct Communication (Asymmetric Naming)
 * Operating System Concepts, 10th Ed.
 *
 * File 1 of 2: Sender Process
 * Names recipient queue and attaches its own PID: send(P, message)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME "/os_asymmetric_queue"

typedef struct {
    pid_t sender_pid;
    char text[128];
} asym_msg_t;

int main(int argc, char *argv[])
{
    pid_t my_pid = getpid();
    const char *payload = (argc > 1) ? argv[1] : "Payload from asymmetric sender";

    mqd_t mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open (sender) failed");
        fprintf(stderr, "Hint: Start ./08_asymmetric_comm_receiver first!\n");
        return 1;
    }

    asym_msg_t msg;
    msg.sender_pid = my_pid;
    snprintf(msg.text, sizeof(msg.text), "%s", payload);

    printf("[Sender Process PID: %d] Sending message: \"%s\"\n", my_pid, msg.text);

    if (mq_send(mq, (const char *)&msg, sizeof(asym_msg_t), 1) == -1) {
        perror("mq_send failed");
        mq_close(mq);
        return 1;
    }

    printf("[Sender Process PID: %d] Message sent to queue.\n", my_pid);
    mq_close(mq);
    return 0;
}
