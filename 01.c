#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SIZE 1024
#define PORT 8090

int main()
{
    int socket_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    FILE *file;
    int bytes;

    char filename[SIZE];
    char message[SIZE];

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        perror("socket failed");
        exit(1);
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if(bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(1);
    }
    if(listen(socket_fd, 3) < 0)
    {
        perror("listen failed");
        exit(1);
    }
    printf("server waiting for client\n");

    new_socket = accept(socket_fd, (struct sockaddr *)&address, &addrlen);
    if(new_socket < 0)
    {
        perror("accept failed");
        exit(1);
    }
    printf("connected\n");
    memset(filename, 0, sizeof(filename));
    recv(new_socket, filename, sizeof(filename), 0);
    printf("client requested the file: %s", filename);

    file = fopen(filename, "r");

    if(file == NULL)
    {
        perror("file not found\n");
        strcpy(message, "file not found\n");
        send(new_socket, message, sizeof(message), 0);
        close(socket_fd);
        close(new_socket);
    }

    while((bytes = fread(message, 1, sizeof(message), file)) > 0)
    {
        send(new_socket, message, bytes, 0);
    }
    printf("files sent successfully");
    close(socket_fd);
    close(new_socket);
    fclose(file);
    return 0;
}