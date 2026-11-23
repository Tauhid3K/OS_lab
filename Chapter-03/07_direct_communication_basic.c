// Direct Communication between Processes

#include <stdio.h>

int main(void)
{
    int message = 50;

    // Sender Process
    printf("Process A is Sending Message to Process B\n");

    // send(B, message)
    printf("Message Sent: %d\n", message);

    // Receiver Process B
    printf("Process B Received Message: %d\n", message);

    return 0;
}
