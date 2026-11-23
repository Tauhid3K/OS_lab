#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Programming Problem 3.27 (Operating System Concepts, 10th Ed.)
 *
 * filecopy.c:
 * Copies a file using an ordinary pipe.
 *
 * Parent process reads content from the source file and writes it into the pipe.
 * Child process reads data from the pipe and writes it to the destination file.
 *
 * Usage: ./problem_3_27_filecopy <source_file> <destination_file>
 */

#define BUFFER_SIZE 512
#define READ_END  0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    char *src_filename = argv[1];
    char *dest_filename = argv[2];

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid > 0) {
        // Parent Process: Reads source file and writes into pipe
        close(fd[READ_END]);

        int src_fd = open(src_filename, O_RDONLY);
        if (src_fd < 0) {
            perror("Failed to open source file");
            close(fd[WRITE_END]);
            wait(NULL);
            return 1;
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
            write(fd[WRITE_END], buffer, bytes_read);
        }

        close(src_fd);
        close(fd[WRITE_END]); // Child receives EOF

        wait(NULL);
        printf("[filecopy] File copy completed successfully: '%s' -> '%s'\n",
               src_filename, dest_filename);
    }
    else {
        // Child Process: Reads from pipe and writes to destination file
        close(fd[WRITE_END]);

        int dest_fd = open(dest_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd < 0) {
            perror("Failed to open/create destination file");
            close(fd[READ_END]);
            exit(1);
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(fd[READ_END], buffer, sizeof(buffer))) > 0) {
            write(dest_fd, buffer, bytes_read);
        }

        close(dest_fd);
        close(fd[READ_END]);
        exit(0);
    }

    return 0;
}
