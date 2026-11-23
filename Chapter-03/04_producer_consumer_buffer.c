#include <stdio.h>

#define BUFFER_SIZE 10

// Structure for storing an item
typedef struct {
    int data;
} item;

// Shared Buffer
item buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// Check whether buffer is full
int isFull(void)
{
    return (in + 1) % BUFFER_SIZE == out;
}

// Check whether buffer is empty
int isEmpty(void)
{
    return in == out;
}

// Producer Function
void producer(int value)
{
    if (isFull()) {
        printf("Buffer is Full! Producer must wait.\n");
    } else {
        item next_produced;
        next_produced.data = value;
        buffer[in] = next_produced;
        printf("Produced: %d\n", value);
        in = (in + 1) % BUFFER_SIZE;
    }
}

// Consumer Function
void consumer(void)
{
    if (isEmpty()) {
        printf("Buffer is Empty! Consumer must wait.\n");
    } else {
        item next_consumed;
        next_consumed = buffer[out];
        printf("Consumed: %d\n", next_consumed.data);
        out = (out + 1) % BUFFER_SIZE;
    }
}

int main(void)
{
    printf("Producer-Consumer Problem (Bounded Buffer)\n");
    printf("------------------------------------------\n");

    // Producer produces 10 items
    printf("\nProducer Working:\n");
    for (int i = 1; i <= 10; i++) {
        producer(i);
    }

    // Consumer consumes 3 items
    printf("\nConsumer Working:\n");
    for (int i = 1; i <= 3; i++) {
        consumer();
    }

    // Producer produces more items
    printf("\nProducer Working Again:\n");
    producer(11);
    producer(12);

    // Consumer consumes remaining items
    printf("\nConsumer Working Again:\n");
    for (int i = 1; i <= 10; i++) {
        consumer();
    }

    return 0;
}
