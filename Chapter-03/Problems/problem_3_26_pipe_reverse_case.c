#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Programming Problem 3.26 (Operating System Concepts, 10th Ed.)
 *
 * Two Ordinary Pipes IPC:
 * 1. Parent sends a string to the child across pipe 1.
 * 2. Child reads string from pipe 1, reverses the case of each character
 *    (uppercase to lowercase and lowercase to uppercase).
 * 3. Child sends inverted string back to parent across pipe 2.
 * 4. Parent reads inverted string from pipe 2 and prints it.
 */

#define BUFFER_SIZE 128
#define READ_END  0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    int pipe1[2]; // Parent -> Child
    int pipe2[2]; // Child -> Parent

    char message[BUFFER_SIZE] = "Hello Operating Systems 10th Edition!";
    if (argc > 1) {
        strncpy(message, argv[1], BUFFER_SIZE - 1);
        message[BUFFER_SIZE - 1] = '\0';
    }

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid > 0) {
        // Parent Process
        // Close unused ends: read on pipe1, write on pipe2
        close(pipe1[READ_END]);
        close(pipe2[WRITE_END]);

        printf("Parent (PID %d): Original message: '%s'\n", getpid(), message);
        printf("Parent: Sending to child via Pipe 1...\n");
        write(pipe1[WRITE_END], message, strlen(message) + 1);
        close(pipe1[WRITE_END]); // Finished writing

        char response[BUFFER_SIZE];
        read(pipe2[READ_END], response, BUFFER_SIZE);
        close(pipe2[READ_END]);

        wait(NULL);

        printf("Parent: Received from child via Pipe 2: '%s'\n", response);
    }
    else {
        // Child Process
        // Close unused ends: write on pipe1, read on pipe2
        close(pipe1[WRITE_END]);
        close(pipe2[READ_END]);

        char buffer[BUFFER_SIZE];
        read(pipe1[READ_END], buffer, BUFFER_SIZE);
        close(pipe1[READ_END]);

        printf("Child (PID %d): Received '%s', inverting case...\n", getpid(), buffer);

        // Invert character case
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (isupper((unsigned char)buffer[i])) {
                buffer[i] = tolower((unsigned char)buffer[i]);
            } else if (islower((unsigned char)buffer[i])) {
                buffer[i] = toupper((unsigned char)buffer[i]);
            }
        }

        // Send back across pipe 2
        write(pipe2[WRITE_END], buffer, strlen(buffer) + 1);
        close(pipe2[WRITE_END]);

        exit(0);
    }

    return 0;
}
