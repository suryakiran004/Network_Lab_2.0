#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8090
#define BUFFER_SIZE 1024

int main()
{
    int sockfd, newsock;
    struct sockaddr_in address;
    socklen_t len = sizeof(address);

    FILE *file;
    int bytes;

    char filename[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(1);
    }

    // Listen
    if (listen(sockfd, 3) < 0)
    {
        perror("Listen failed");
        exit(1);
    }

    printf("Server waiting for connection...\n");

    // Accept client
    newsock = accept(sockfd, (struct sockaddr *)&address, &len);

    if (newsock < 0)
    {
        perror("Accept failed");
        exit(1);
    }

    printf("Server connected...\n");

    // Clear filename buffer
    memset(filename, 0, BUFFER_SIZE);

    // Receive filename
    recv(newsock, filename, BUFFER_SIZE, 0);

    printf("Client requested file: %s\n", filename);

    // Open file
    file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("fopen failed");

        strcpy(message, "File not found\n");

        send(newsock, message, strlen(message), 0);

        close(newsock);
        close(sockfd);

        return 0;
    }

    // Read file and send contents
    while ((bytes = fread(message, 1, BUFFER_SIZE, file)) > 0)
    {
        send(newsock, message, bytes, 0);
    }

    printf("File sent successfully.\n");

    fclose(file);

    close(newsock);
    close(sockfd);

    return 0;
}