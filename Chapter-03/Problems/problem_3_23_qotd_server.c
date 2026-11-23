#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
 * Programming Problem 3.23 (Operating System Concepts, 10th Ed.)
 * Quote-of-the-Day (QOTD) TCP Server listening on port 6017.
 *
 * When a client connects, the server delivers a quote and closes the connection.
 * Test with: nc 127.0.0.1 6017   or: telnet 127.0.0.1 6017
 */

#define PORT 6017

static const char *quotes[] = {
    "The secret of getting ahead is getting started. - Mark Twain\n",
    "Simplicity is prerequisite for reliability. - Edsger W. Dijkstra\n",
    "Talk is cheap. Show me the code. - Linus Torvalds\n",
    "There are only two hard things in Computer Science: cache invalidation and naming things. - Phil Karlton\n",
    "An operating system is a program that acts as an intermediary between a user and computer hardware. - Silberschatz\n"
};

#define NUM_QUOTES (sizeof(quotes) / sizeof(quotes[0]))

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

    printf("[QOTD Server] Listening on port %d. (Press Ctrl+C to stop)\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        const char *selected_quote = quotes[rand() % NUM_QUOTES];
        send(client_fd, selected_quote, strlen(selected_quote), 0);
        printf("[QOTD Server] Sent quote to client.\n");

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
