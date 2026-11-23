#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * End-of-Chapter Exercise 3.12 / Figure 3.33 (Operating System Concepts, 10th Ed.)
 *
 * Question: Under what circumstances would "LINE J" be reached?
 *
 * Explanation:
 * The execlp() system call completely replaces the address space of the process
 * with the new executable program ("/bin/ls").
 * If execlp() succeeds, it never returns, so "LINE J" is NEVER reached.
 * "LINE J" is reached ONLY IF execlp() encounters an error and returns -1
 * (e.g., if the file doesn't exist, lacks execute permissions, or memory is exhausted).
 */

int main(int argc, char *argv[])
{
    (void)argv;
    pid_t pid;

    int test_failure = (argc > 1);

    printf("--- Exercise 3.12 (Figure 3.33) Execution ---\n");
    if (test_failure) {
        printf("Mode: Testing FAILURE case (calling nonexistent command)...\n");
    } else {
        printf("Mode: Testing NORMAL case (run './exercise_3_12_exec_line_j fail' to test failure)\n");
    }

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        if (test_failure) {
            // Intentionally invalid executable to demonstrate LINE J execution
            execlp("/bin/nonexistent_command_xyz", "nonexistent", NULL);
        } else {
            // Normal execution
            execlp("/bin/ls", "ls", NULL);
        }

        // LINE J is reached ONLY if execlp() fails!
        printf("LINE J reached! (execlp failed to execute)\n");
        return 1;
    }
    else {
        // Parent process
        wait(NULL);
        printf("\nParent: Child process finished.\n");
    }

    return 0;
}
