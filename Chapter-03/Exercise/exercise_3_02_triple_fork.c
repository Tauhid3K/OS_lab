#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Practice Exercise 3.2 / Figure 3.31 (Operating System Concepts, 10th Ed.)
 *
 * Question: How many unique processes are created by the program?
 */

int main(void)
{
    printf("--- Practice Exercise 3.2 (Figure 3.31) Execution ---\n");
    printf("Initial process PID: %d\n\n", getpid());

    fork();
    fork();
    fork();

    printf("Process alive: PID = %d, Parent PID = %d\n", getpid(), getppid());

    // Wait for any child processes to prevent zombies
    while (wait(NULL) > 0);

    return 0;
}
