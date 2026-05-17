#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8070
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char filename[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    FILE *file;
    int bytes;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server waiting for connection...\n");

    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    memset(filename, 0, BUFFER_SIZE);
    recv(client_socket, filename, BUFFER_SIZE, 0);

    printf("Client requested file: %s\n", filename);

    file = fopen(filename, "r");

    if (file == NULL)
    {
        strcpy(buffer, "File not found\n");
        send(client_socket, buffer, strlen(buffer), 0);
    }
    else
    {
        while ((bytes = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
        {
            send(client_socket, buffer, bytes, 0);
        }
        fclose(file);
    }

    close(client_socket);
    close(server_fd);

    return 0;
}
