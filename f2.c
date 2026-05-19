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
    int sockfd;

    struct sockaddr_in servaddr;

    char filename[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    int bytes;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0)
    {
        perror("Invalid address");
        exit(1);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Connection failed");
        exit(1);
    }

    printf("Client connected...\n");

    // Input filename
    printf("Enter the file name: ");
    scanf("%s", filename);

    // Send filename (+1 sends '\0')
    send(sockfd, filename, strlen(filename) + 1, 0);

    printf("\nFile content:\n\n");

    // Receive file contents
    while ((bytes = recv(sockfd, message, BUFFER_SIZE, 0)) > 0)
    {
        fwrite(message, 1, bytes, stdout);
    }

    printf("\n");

    close(sockfd);

    return 0;
}