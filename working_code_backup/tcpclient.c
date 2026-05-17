#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024];
    char message[1024];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("Connected to server!\n");

    while (1) {
        printf("Client: ");
        fgets(message, sizeof(message), stdin);
        send(client_fd, message, strlen(message), 0);

        if (strncmp(message, "exit", 4) == 0) break;

        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, sizeof(buffer));
        printf("Server: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) break;
    }

    close(client_fd);
    return 0;
}

