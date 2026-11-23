//This program demonstrates the conceptual lifecycle of a mailbox:
//Create
//Send
//Receive
//Delete

#include <stdio.h>

int main(void)
{
    int mailbox = 0;
    int message = 500;

    // Create Mailbox
    printf("Mailbox Created\n");

    // Send Message
    mailbox = message;
    printf("Message Sent: %d\n", message);

    // Receive Message
    printf("Message Received: %d\n", mailbox);

    // Delete Mailbox
    mailbox = 0;
    printf("Mailbox Deleted\n");

    return 0;
}
