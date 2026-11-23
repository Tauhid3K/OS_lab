#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Programming Problem 3.21 (Operating System Concepts, 10th Ed.)
 *
 * The Collatz conjecture:
 * For any positive integer n:
 *   - If n is even: n = n / 2
 *   - If n is odd:  n = 3 * n + 1
 * Sequence always terminates at 1.
 *
 * This program generates the sequence in a child process created by fork().
 * The parent waits for the child to terminate before exiting.
 *
 * Usage: ./problem_3_21_collatz_fork <positive integer>
 * Example: ./problem_3_21_collatz_fork 35
 */

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Error: Argument must be a positive integer (> 0).\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process calculates and outputs the Collatz sequence
        printf("[Child PID %d] Collatz sequence for %d:\n", getpid(), n);
        printf("%d", n);

        while (n != 1) {
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = 3 * n + 1;
            }
            printf(", %d", n);
        }
        printf("\n");
        exit(0);
    }
    else {
        // Parent process waits for child to complete
        wait(NULL);
        printf("[Parent PID %d] Child process finished.\n", getpid());
    }

    return 0;
}
