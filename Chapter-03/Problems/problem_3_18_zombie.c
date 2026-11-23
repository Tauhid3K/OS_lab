#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/*
 * Programming Problem 3.18 (Operating System Concepts, 10th Ed.)
 * Create a zombie process that remains in the system for at least 10 seconds.
 *
 * How it works:
 * 1. Parent forks child.
 * 2. Child exits immediately with exit(0).
 * 3. Parent does NOT call wait(), but instead sleeps for 15 seconds.
 * 4. While parent sleeps, child entry remains in process table as a zombie ("<defunct>").
 */

int main(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process exits immediately
        printf("[Child PID %d] Exiting now. I am now a zombie process!\n", getpid());
        exit(0);
    }
    else {
        // Parent process sleeps without calling wait()
        printf("[Parent PID %d] Child PID is %d.\n", getpid(), pid);
        printf("[Parent] Sleeping for 15 seconds...\n");
        printf("[Parent] Open another terminal and run:\n");
        printf("         ps -l | grep 'Z'\n");
        printf("         or: ps aux | grep -w 'Z'\n\n");

        sleep(15);

        printf("[Parent] Waking up. Exiting now, so child zombie will be reaped by init/systemd.\n");
    }

    return 0;
}
