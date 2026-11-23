#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
 * Programming Problem 3.24 (Operating System Concepts, 10th Ed.)
 * Haiku TCP Server listening on port 5575.
 *
 * When a client connects, the server sends a 3-line Haiku poem (5-7-5 syllables).
 * Test with: nc 127.0.0.1 5575   or: telnet 127.0.0.1 5575
 */

#define PORT 5575

static const char *haikus[] = {
    "Forking processes,\nSharing data in the cloud,\nCode runs wild and free.\n",
    "Waiting for the child,\nParent sleeps in quiet grace,\nZombie reaped at last.\n",
    "Bytes flow down the pipe,\nOne end writes and one end reads,\nData finds its home.\n"
};

#define NUM_HAIKUS (sizeof(haikus) / sizeof(haikus[0]))

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    srand((unsigned int)time(NULL));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        return 1;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        return 1;
    }

    printf("[Haiku Server] Listening on port %d. (Press Ctrl+C to stop)\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        const char *poem = haikus[rand() % NUM_HAIKUS];
        send(client_fd, poem, strlen(poem), 0);
        printf("[Haiku Server] Delivered haiku to client.\n");

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
