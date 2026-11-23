//This program demonstrates that arrays are also copied when a process forks.

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 5

/*
 * End-of-Chapter Exercise 3.16 / Figure 3.35 (Operating System Concepts, 10th Ed.)
 *
 * Question: What are the contents of nums array at LINE X (child) and LINE Y (parent)?
 */

int nums[SIZE] = {0, 1, 2, 3, 4};

int main(void)
{
    int i;
    pid_t pid;

    printf("--- Exercise 3.16 (Figure 3.35) Execution ---\n\n");

    pid = fork();

    if (pid == 0) {
        // Child Process
        for (i = 0; i < SIZE; i++) {
            nums[i] *= -i;
            printf("CHILD: %d ", nums[i]); /* LINE X */
        }
        printf("\n");
        return 0;
    }
    else if (pid > 0) {
        // Parent Process
        wait(NULL);
        for (i = 0; i < SIZE; i++) {
            printf("PARENT: %d ", nums[i]); /* LINE Y */
        }
        printf("\n");
    }
    else {
        perror("fork failed");
        return 1;
    }

    return 0;
}
