#include <stdio.h>
#include <stdlib.h>

/*
 * Programming Problem 3.20 (Operating System Concepts, 10th Ed.)
 *
 * Design a PID manager that allocates and deallocates process identifiers.
 * PID range: MIN_PID = 300 to MAX_PID = 5000.
 *
 * Required functions:
 *   int allocate_map(void)   - Creates and initializes data structure (1 success, -1 failure)
 *   int allocate_pid(void)   - Allocates and returns a PID (or -1 if all are in use)
 *   void release_pid(int pid)- Releases a PID
 */

#define MIN_PID 300
#define MAX_PID 5000
#define NUM_PIDS (MAX_PID - MIN_PID + 1)

// Bitmap array where each bit represents the availability of a PID
// 0 = available, 1 = allocated
static unsigned char *pid_bitmap = NULL;

int allocate_map(void)
{
    // Calculate bytes needed: ceil(NUM_PIDS / 8)
    int num_bytes = (NUM_PIDS + 7) / 8;
    pid_bitmap = (unsigned char *)calloc(num_bytes, sizeof(unsigned char));

    if (!pid_bitmap) {
        return -1; // Allocation failure
    }
    return 1; // Success
}

int allocate_pid(void)
{
    if (!pid_bitmap) {
        return -1; // Map not initialized
    }

    for (int i = 0; i < NUM_PIDS; i++) {
        int byte_index = i / 8;
        int bit_offset = i % 8;

        // Check if bit is 0 (available)
        if (!(pid_bitmap[byte_index] & (1 << bit_offset))) {
            // Mark bit as 1 (allocated)
            pid_bitmap[byte_index] |= (1 << bit_offset);
            return MIN_PID + i;
        }
    }

    return -1; // All PIDs currently in use
}

void release_pid(int pid)
{
    if (!pid_bitmap || pid < MIN_PID || pid > MAX_PID) {
        fprintf(stderr, "release_pid: Invalid PID %d\n", pid);
        return;
    }

    int offset = pid - MIN_PID;
    int byte_index = offset / 8;
    int bit_offset = offset % 8;

    // Clear bit to 0 (available)
    pid_bitmap[byte_index] &= ~(1 << bit_offset);
}

// Test harness demonstrating PID allocation and release
int main(void)
{
    printf("=== Programming Problem 3.20: PID Manager Test Harness ===\n\n");

    if (allocate_map() == -1) {
        fprintf(stderr, "Failed to initialize PID bitmap.\n");
        return 1;
    }
    printf("PID bitmap initialized for range [%d, %d] (%d total PIDs).\n\n",
           MIN_PID, MAX_PID, NUM_PIDS);

    // Allocate 5 PIDs
    int pids[5];
    for (int i = 0; i < 5; i++) {
        pids[i] = allocate_pid();
        printf("Allocated PID %d: %d\n", i + 1, pids[i]);
    }

    // Release PID 2 and 4
    printf("\nReleasing PID: %d\n", pids[1]);
    release_pid(pids[1]);
    printf("Releasing PID: %d\n", pids[3]);
    release_pid(pids[3]);

    // Allocate again: should re-use the lowest available released PIDs
    int new_pid1 = allocate_pid();
    int new_pid2 = allocate_pid();
    printf("\nRe-allocated PID: %d (expected %d)\n", new_pid1, pids[1]);
    printf("Re-allocated PID: %d (expected %d)\n", new_pid2, pids[3]);

    free(pid_bitmap);
    printf("\nPID Manager test completed successfully.\n");
    return 0;
}
