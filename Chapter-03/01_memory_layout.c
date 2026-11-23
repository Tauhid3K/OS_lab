#include <stdio.h>
#include <stdlib.h>

// Uninitialized data segment (BSS)
int uninitialized_global;
// BSS contains global and static variables

// Initialized data segment
int initialized_global = 15;

int main(void)
{
    // Stack memory = local variables
    int stack_var = 10;

    // Heap memory
    int *heap_var = (int *)malloc(5 * sizeof(int));
    if (!heap_var) {  // malloc = returns a pointer to the allocated memory
        perror("malloc failed");
        return 1;
    }

    for (int j = 0; j < 5; j++) {
        heap_var[j] = j; // Writing values to the heap memory
    }

    printf("================ Memory Layout of C Program ================\n");
    printf("Text Segment (Code)         : %p (main function)\n", (void *)main);
    printf("Initialized Data Segment    : %p (value = %d)\n", (void *)&initialized_global, initialized_global);
    printf("BSS Segment (Uninitialized) : %p (value = %d)\n", (void *)&uninitialized_global, uninitialized_global);
    printf("Heap Segment (malloc)       : %p\n", (void *)heap_var);
    printf("Stack Segment (local var)   : %p\n", (void *)&stack_var);
    printf("============================================================\n");

    free(heap_var); //Dynamically allocated memory should be freed to avoid memory leaks
    return 0;
}
