#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

/*
 * Programming Problem 3.19 (Part 1 - Shared Memory Version)
 * (Operating System Concepts, 10th Ed.)
 *
 * time_shm.c:
 * Runs a specified command and determines elapsed time.
 * Uses POSIX shared memory (shm_open, ftruncate, mmap) to pass the start time
 * from the child process to the parent process.
 *
 * Usage: ./problem_3_19_time_shm <command> [args...]
 * Example: ./problem_3_19_time_shm ls -la
 */

#define SHM_NAME "/time_shm_demo"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [arguments...]\n", argv[0]);
        return 1;
    }

    const int SIZE = sizeof(struct timeval);

    // Create shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        return 1;
    }
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate failed");
        return 1;
    }

    // Map shared memory
    struct timeval *start_time = (struct timeval *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (start_time == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child: record starting time in shared memory before exec
        gettimeofday(start_time, NULL);

        // Execute command
        execvp(argv[1], &argv[1]);

        perror("execvp failed");
        exit(1);
    }
    else {
        // Parent: wait for child to finish execution
        wait(NULL);

        // Record end time
        struct timeval end_time;
        gettimeofday(&end_time, NULL);

        // Calculate elapsed time
        long seconds = end_time.tv_sec - start_time->tv_sec;
        long microseconds = end_time.tv_usec - start_time->tv_usec;
        if (microseconds < 0) {
            seconds -= 1;
            microseconds += 1000000;
        }

        double elapsed = seconds + (microseconds / 1000000.0);
        printf("\n[time_shm] Elapsed time: %.5f seconds\n", elapsed);

        // Clean up shared memory
        munmap(start_time, SIZE);
        close(shm_fd);
        shm_unlink(SHM_NAME);
    }

    return 0;
}
