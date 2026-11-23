//This program opens the shared-memory object created by the producer and reads its content.

#include <stdio.h>
#include <stdlib.h>
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

    // Open existing shared memory object
    fd = shm_open(name, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open failed (did you run ./12_posix_shm_producer first?)");
        return 1;
    }

    // Memory map the shared memory object
    ptr = (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // Read and display data from shared memory
    printf("Consumer: Shared memory opened.\n");
    printf("Consumer: Read data: '%s'\n", ptr);

    // Unlink shared memory object
    if (shm_unlink(name) == -1) {
        perror("shm_unlink failed");
        return 1;
    }

    printf("Consumer: Shared memory unlinked successfully.\n");

    return 0;
}
