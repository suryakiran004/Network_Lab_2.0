#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int client_fd;
    struct sockaddr_in server_address;
    char buffer[1024];
    char message[1024];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(client_fd, "127.0.0.1", &server_address.sin_addr);

    connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    printf("Connected to server...\n");

    while(1)
    {
        printf("Client: ");
        fgets(message, sizeof(message), stdin);
        send(client_fd, message, strlen(message), 0);

        if(strncmp(message, "exit", 4)==0) break;

        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, sizeof(buffer));
        printf("Server: %s", buffer);

        if(strncmp(buffer, "exit", 4)==0) break;
    }
    close(client_fd);
    return 0;
}