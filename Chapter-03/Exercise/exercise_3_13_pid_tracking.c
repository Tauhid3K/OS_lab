#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * End-of-Chapter Exercise 3.13 / Figure 3.34 (Operating System Concepts, 10th Ed.)
 *
 * Question: Assuming parent PID = 2600 and child PID = 2603,
 * what are the values of pid and pid1 at lines A, B, C, and D?
 *
 * Analysis:
 * Line A (Child):  pid  = 0     (fork() returns 0 to the child)
 * Line B (Child):  pid1 = 2603  (getpid() inside child returns its own PID)
 * Line C (Parent): pid  = 2603  (fork() returns child's PID to parent)
 * Line D (Parent): pid1 = 2600  (getpid() inside parent returns parent's own PID)
 */

int main(void)
{
    pid_t pid, pid1;

    printf("--- Exercise 3.13 (Figure 3.34) Execution ---\n\n");

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0) {
        // Child Process
        pid1 = getpid();
        printf("Child Process:\n");
        printf("  LINE A: pid  = %d  (Expected in book example: 0)\n", pid);
        printf("  LINE B: pid1 = %d  (Expected in book example: 2603 [child PID])\n", pid1);
    }
    else {
        // Parent Process
        pid1 = getpid();
        printf("Parent Process:\n");
        printf("  LINE C: pid  = %d  (Expected in book example: 2603 [child PID])\n", pid);
        printf("  LINE D: pid1 = %d  (Expected in book example: 2600 [parent PID])\n\n", pid1);
        wait(NULL);
    }

    return 0;
}
