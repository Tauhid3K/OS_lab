#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(void)
{
    const int SIZE = 4096;
    const char *name = "OS_SHM";

    int fd;
    char *ptr;

    // Create shared memory object
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    // Configure size of shared memory
    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate failed");
        return 1;
    }

    // Memory map the shared memory object
    ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // Write message into shared memory
    sprintf(ptr, "Hello World from POSIX Shared Memory!");

    printf("Producer: Shared memory created and mapped.\n");
    printf("Producer: Data written: '%s'\n", ptr);
    printf("Producer: Run ./13_posix_shm_consumer to read this data.\n");

    return 0;
}
