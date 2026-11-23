#include <stdio.h>

int main(void)
{
    int message;

    // Sender
    message = 100;
    printf("Sender: Sending Message = %d\n", message);

    // Receiver
    printf("Receiver: Received Message = %d\n", message);

    return 0;
}
