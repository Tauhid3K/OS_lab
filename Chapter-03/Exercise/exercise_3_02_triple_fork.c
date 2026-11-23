#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Practice Exercise 3.2 / Figure 3.31 (Operating System Concepts, 10th Ed.)
 *
 * Question: How many unique processes are created by the program?
 * Explanation:
 * Each fork() duplicates every active process.
 * Initial: 1 process
 * 1st fork(): 2^1 = 2 processes
 * 2nd fork(): 2^2 = 4 processes
 * 3rd fork(): 2^3 = 8 processes total (1 initial parent + 7 children).
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
