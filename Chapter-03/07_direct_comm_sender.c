/*
 * Section 3.5.2: Direct Communication (Symmetric Naming)
 * Operating System Concepts, 10th Ed.
 *
 * File 1 of 2: Sender Process
 * Explicitly addresses the recipient by its PID: send(P, message)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main(int argc, char *argv[])
{
    pid_t my_pid = getpid();
    pid_t target_pid = 0;

    // Determine target receiver PID (from argument or rendezvous file)
    if (argc >= 2) {
        target_pid = (pid_t)atoi(argv[1]);
    } else {
        FILE *f = fopen(PID_FILE, "r");
        if (f) {
            if (fscanf(f, "%d", &target_pid) != 1) {
                target_pid = 0;
            }
            fclose(f);
        }
    }

    if (target_pid <= 0) {
        fprintf(stderr, "Receiver PID not found!\n");
        fprintf(stderr, "Usage: ./07_direct_comm_sender <RECEIVER_PID>\n");
        fprintf(stderr, "Hint: Run ./07_direct_comm_receiver first to start the receiver.\n");
        return 1;
    }

    mqd_t mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open (sender) failed");
        fprintf(stderr, "Make sure ./07_direct_comm_receiver is running first!\n");
        return 1;
    }

    printf("===================================================\n");
    printf(" Direct Symmetric Communication: Sender [PID: %d]\n", my_pid);
    printf(" Explicitly Target Process PID: %d\n", target_pid);
    printf("===================================================\n\n");

    direct_msg_t msg;
    msg.sender_pid = my_pid;
    msg.target_pid = target_pid;
    snprintf(msg.text, sizeof(msg.text), "Direct symmetric message to PID %d", target_pid);

    printf("[Sender PID %d] Calling send(%d, msg): \"%s\"\n", my_pid, target_pid, msg.text);

    if (mq_send(mq, (const char *)&msg, sizeof(direct_msg_t), 1) == -1) {
        perror("mq_send failed");
        mq_close(mq);
        return 1;
    }

    printf("[Sender PID %d] Message sent successfully to target PID %d.\n", my_pid, target_pid);

    mq_close(mq);
    return 0;
}
