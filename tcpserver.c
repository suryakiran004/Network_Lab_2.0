#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024];
    char message[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server waiting for connection...\n");
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    printf("Client connected!\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, sizeof(buffer));
        printf("Client: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) break;

        printf("Server: ");
        fgets(message, sizeof(message), stdin);
        send(new_socket, message, strlen(message), 0);

        if (strncmp(message, "exit", 4) == 0) break;
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

