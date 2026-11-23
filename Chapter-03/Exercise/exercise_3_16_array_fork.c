#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 5

/*
 * End-of-Chapter Exercise 3.16 / Figure 3.35 (Operating System Concepts, 10th Ed.)
 *
 * Question: What are the contents of nums array at LINE X (child) and LINE Y (parent)?
 *
 * Analysis:
 * Initial array: [0, 1, 2, 3, 4]
 * Child loops through and modifies: nums[i] *= -i;
 *   nums[0] = 0 * -0 = 0
 *   nums[1] = 1 * -1 = -1
 *   nums[2] = 2 * -2 = -4
 *   nums[3] = 3 * -3 = -9
 *   nums[4] = 4 * -4 = -16
 *
 * Output at LINE X (Child): 0 -1 -4 -9 -16
 *
 * Because child gets its own separate copy of memory, the parent's nums array
 * is NOT affected!
 * Output at LINE Y (Parent): 0 1 2 3 4
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
