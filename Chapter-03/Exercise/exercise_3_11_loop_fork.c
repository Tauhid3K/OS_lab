#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * End-of-Chapter Exercise 3.11 / Figure 3.32 (Operating System Concepts, 10th Ed.)
 *
 * Question: Including the initial parent process, how many processes
 * are created by the character program with 4 forks in a loop?
 *
 * Explanation:
 * Loop runs 4 times:
 * i = 0: 2^1 = 2 processes
 * i = 1: 2^2 = 4 processes
 * i = 2: 2^3 = 8 processes
 * i = 3: 2^4 = 16 processes
 * Total processes created = 2^4 = 16 processes (1 original parent + 15 descendants).
 */

int main(void)
{
    printf("--- Exercise 3.11 (Figure 3.32) Execution ---\n");
    printf("Original parent PID: %d\n\n", getpid());

    int i;
    for (i = 0; i < 4; i++) {
        fork();
    }

    printf("Process alive: PID %d, Parent PID %d\n", getpid(), getppid());

    // Clean up child processes
    while (wait(NULL) > 0);

    return 0;
}
