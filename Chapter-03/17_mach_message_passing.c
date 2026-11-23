#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Figure 3.18 & Mach IPC Overview (Operating System Concepts, 10th Ed.)
 * Demonstrates the structure of Mach message passing (mach_msg()).
 * On macOS/Mach systems, <mach/mach.h> provides mach_msg().
 * On standard Linux systems, this program models the exact message structure
 * and semantics described in Section 3.6.1.
 */

typedef struct {
    unsigned int msgh_bits;
    unsigned int msgh_size;
    unsigned int msgh_remote_port;
    unsigned int msgh_local_port;
    unsigned int msgh_id;
} mach_msg_header_t;

typedef struct {
    mach_msg_header_t header;
    int data;
} message_t;

int main(void)
{
    printf("=== Mach IPC Message Structure Demonstration (Section 3.6.1 / Figure 3.18) ===\n\n");

    message_t msg;
    msg.header.msgh_size = sizeof(message_t);
    msg.header.msgh_remote_port = 1001; // target receiver port
    msg.header.msgh_local_port = 1002;  // reply port
    msg.header.msgh_id = 1;             // message type id
    msg.data = 42;                      // payload

    printf("Constructed Mach Message:\n");
    printf("  Header Size:        %u bytes\n", msg.header.msgh_size);
    printf("  Remote (Dest) Port: %u\n", msg.header.msgh_remote_port);
    printf("  Local (Reply) Port: %u\n", msg.header.msgh_local_port);
    printf("  Message ID:         %u\n", msg.header.msgh_id);
    printf("  Payload Data:       %d\n\n", msg.data);

    printf("Mach IPC Mechanics:\n");
    printf("  - Ports are kernel-protected message queues.\n");
    printf("  - Sender calls:   mach_msg(&msg.header, MACH_SEND_MSG, ...)\n");
    printf("  - Receiver calls: mach_msg(&msg.header, MACH_RCV_MSG, ...)\n");
    printf("  - If the receiver's port queue is full, sender can block, wait up to n ms, or cache.\n");

    return 0;
}
