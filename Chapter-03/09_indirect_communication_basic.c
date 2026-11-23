//Indirect communication uses a mailbox or message queue as an intermediary.

#include <stdio.h>

int main(void)
{
    int mailbox;
    int message = 300;

    // Send message to mailbox
    mailbox = message;
    printf("Message Sent to Mailbox\n");

    // Receive message from mailbox
    printf("Message Received: %d\n", mailbox);

    return 0;
}
