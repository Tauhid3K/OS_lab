#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Programming Problem 3.19 (Part 2 - Ordinary Pipe Version)
 * (Operating System Concepts, 10th Ed.)
 *
 * time_pipe.c:
 * Runs a specified command and determines elapsed time.
 * Uses a standard UNIX pipe to pass the start time from child to parent.
 *
 * Usage: ./problem_3_19_time_pipe <command> [args...]
 * Example: ./problem_3_19_time_pipe ls -la
 */

#define READ_END  0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [arguments...]\n", argv[0]);
        return 1;
    }

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

    if (pid == 0) {
        // Child process
        close(fd[READ_END]);

        struct timeval start_time;
        gettimeofday(&start_time, NULL);

        // Send start time through the pipe to parent
        write(fd[WRITE_END], &start_time, sizeof(struct timeval));
        close(fd[WRITE_END]);

        // Execute target command
        execvp(argv[1], &argv[1]);

        perror("execvp failed");
        exit(1);
    }
    else {
        // Parent process
        close(fd[WRITE_END]);

        struct timeval start_time;
        read(fd[READ_END], &start_time, sizeof(struct timeval));
        close(fd[READ_END]);

        // Wait for child command to complete
        wait(NULL);

        struct timeval end_time;
        gettimeofday(&end_time, NULL);

        long seconds = end_time.tv_sec - start_time.tv_sec;
        long microseconds = end_time.tv_usec - start_time.tv_usec;
        if (microseconds < 0) {
            seconds -= 1;
            microseconds += 1000000;
        }

        double elapsed = seconds + (microseconds / 1000000.0);
        printf("\n[time_pipe] Elapsed time: %.5f seconds\n", elapsed);
    }

    return 0;
}
