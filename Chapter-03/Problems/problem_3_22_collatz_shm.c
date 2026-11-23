#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

/*
 * Programming Problem 3.22 (Operating System Concepts, 10th Ed.)
 *
 * Collatz sequence using POSIX shared memory:
 * Child process generates Collatz sequence and writes numbers into
 * a POSIX shared-memory object.
 * Parent waits for child to complete, then reads the sequence from
 * shared memory and outputs it, before removing shared memory with shm_unlink().
 *
 * Usage: ./problem_3_22_collatz_shm <positive integer>
 * Example: ./problem_3_22_collatz_shm 35
 */

#define SHM_NAME "/collatz_shm"
#define SHM_SIZE 4096

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Error: Argument must be a positive integer (> 0).\n");
        return 1;
    }

    // Create POSIX shared memory object
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process
        char *ptr = (char *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED) {
            perror("child mmap failed");
            exit(1);
        }

        // Generate Collatz sequence into shared memory buffer
        int offset = sprintf(ptr, "%d", n);

        while (n != 1) {
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = 3 * n + 1;
            }
            offset += sprintf(ptr + offset, ", %d", n);
        }
        sprintf(ptr + offset, "\n");

        munmap(ptr, SHM_SIZE);
        close(fd);
        exit(0);
    }
    else {
        // Parent process waits for child
        wait(NULL);

        // Map shared memory to read result
        char *ptr = (char *)mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED) {
            perror("parent mmap failed");
            return 1;
        }

        printf("Parent (read from shared memory): %s", ptr);

        // Clean up
        munmap(ptr, SHM_SIZE);
        close(fd);
        shm_unlink(SHM_NAME);
    }

    return 0;
}
