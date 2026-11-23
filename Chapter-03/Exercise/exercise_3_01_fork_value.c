#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Practice Exercise 3.1 / Figure 3.30 (Operating System Concepts, 10th Ed.)
 *
 * Question: What will be the output at LINE A?
 * 
 */

int value = 5;

int main(void)
{
    pid_t pid;

    printf("--- Practice Exercise 3.1 (Figure 3.30) Execution ---\n");

    pid = fork();

    if (pid == 0) {
        // Child process
        value += 15;
        printf("CHILD:  value = %d (PID %d)\n", value, getpid());
        return 0;
    }
    else if (pid > 0) {
        // Parent process
        wait(NULL);
        printf("PARENT: value = %d /* LINE A */ (PID %d)\n", value, getpid());
    }
    else {
        perror("fork failed");
        return 1;
    }

    return 0;
}
