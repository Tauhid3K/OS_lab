#include <stdio.h>

int main(void)
{
    int next_produced;

    printf("Producer Process Started\n\n");

    for (int i = 1; i <= 5; i++) {
        // Produce an item
        next_produced = i * 10;
        printf("Produced Item: %d\n", next_produced);

        // Send message
        printf("Message Sent: %d\n\n", next_produced);
    }

    return 0;
}
