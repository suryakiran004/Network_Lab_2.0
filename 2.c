#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define SIZE 1024
#define PORT 8080
#define WINDOW_SIZE 4
#define PACKETS 10
#define TIMEOUT_SECOND 3

int main()
{
    int sock = 0;
    struct sockaddr_in address;
    struct timeval tv;
    char buffer[SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket creation failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
    {
        perror("invalid address");
        exit(1);
    }

    if(connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("connection failed")
        exit(1);
    }
    printf("connected to server\n");

    tv.tv_sec = TIMEOUT_SECOND;
    tv.tv_usec = 0;

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    int base = 1;
    int next = 1;
    int ack, pack_ack =0;
}