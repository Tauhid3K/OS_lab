#include <stdio.h>

int main(void)
{
    int message = 200;
    int id;

    // Sender Process A
    printf("Process A Sending Message\n");

    // Receiver gets message
    printf("Receiver Received Message: %d\n", message);

    // Store sender ID
    id = 1;
    printf("Message Received From Process ID: %d\n", id);

    return 0;
}
