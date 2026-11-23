#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25
#define READ_END  0
#define WRITE_END 1

/*
 * Figure 3.21 & 3.22 (Operating System Concepts, 10th Ed.)
 * UNIX Ordinary Pipe demonstration between parent and child processes.
 */
int main(void)
{
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    pid_t pid;

    // Create the pipe
    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed\n");
        return 1;
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid > 0) {
        // Parent process: writes to the pipe
        // Close unused read end of the pipe
        close(fd[READ_END]);

        printf("Parent (PID %d): Writing message '%s' to pipe...\n", getpid(), write_msg);
        write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);

        // Close write end after sending
        close(fd[WRITE_END]);

        // Wait for child to complete
        wait(NULL);
    }
    else {
        // Child process: reads from the pipe
        // Close unused write end of the pipe
        close(fd[WRITE_END]);

        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("Child (PID %d): Read message '%s' from pipe.\n", getpid(), read_msg);

        // Close read end
        close(fd[READ_END]);
    }

    return 0;
}
