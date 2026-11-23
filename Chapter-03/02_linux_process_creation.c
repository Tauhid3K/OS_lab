#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;

    printf("Parent Process Started (PID: %d)\n", getpid());

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        // Error occurred
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child Process Created (PID: %d, Parent PID: %d)\n", getpid(), getppid());
        printf("Child executing '/bin/ls' via execlp...\n\n");
        execlp("/bin/ls", "ls", "-la", NULL);

        // execlp only returns if an error occurs
        perror("execlp failed");
        exit(1);
    }
    else {
        // Parent process waits for child to complete
        wait(NULL);
        printf("\nChild Complete. Parent Process (PID: %d) Exiting.\n", getpid());
    }

    return 0;
}
