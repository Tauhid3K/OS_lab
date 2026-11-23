#include <stdio.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE] = {10, 20, 30};
int in = 3;
int out = 0;

int main(void)
{
    int next_consumed;

    printf("Consumer Process Started\n\n");

    while (out != in) {  //Check if buffer is not empty
        next_consumed = buffer[out]; 
        // Retrieve the next item to be consumed
        printf("Consumed Item: %d\n", next_consumed);
        out = (out + 1) % BUFFER_SIZE;
        // Move to the next position in the circular buffer
    }

    printf("\nBuffer is Empty!\n");
    return 0;
}
