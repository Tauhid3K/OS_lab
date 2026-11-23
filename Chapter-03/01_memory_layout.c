#include <stdio.h>
#include <stdlib.h>

// Uninitialized data segment (BSS)
int uninitialized_global;

// Initialized data segment
int initialized_global = 15;

int main(void)
{
    // Stack memory
    int stack_var = 10;

    // Heap memory
    int *heap_var = (int *)malloc(5 * sizeof(int));
    if (!heap_var) {
        perror("malloc failed");
        return 1;
    }

    for (int j = 0; j < 5; j++) {
        heap_var[j] = j;
    }

    printf("================ Memory Layout of C Program ================\n");
    printf("Text Segment (Code)         : %p (main function)\n", (void *)main);
    printf("Initialized Data Segment    : %p (value = %d)\n", (void *)&initialized_global, initialized_global);
    printf("BSS Segment (Uninitialized) : %p (value = %d)\n", (void *)&uninitialized_global, uninitialized_global);
    printf("Heap Segment (malloc)       : %p\n", (void *)heap_var);
    printf("Stack Segment (local var)   : %p\n", (void *)&stack_var);
    printf("============================================================\n");

    free(heap_var);
    return 0;
}
