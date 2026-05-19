#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define ACK_PROB 30
#define PORT 8080
#define SIZE 1024

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[SIZE] = {0};
    int ack;
    srand(time(0));
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0)
    {
        perror("socket creation failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(1);
    }

    if(listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(1);
    }

    printf("Server waiting for packets...\n");

    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if(new_socket < 0)
    {
        perror("accept failed");
        exit(1);
    }
    printf("server connection established\n");

    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, sizeof(buffer));
        if(valread == 0) break;
        ack = atoi(buffer);
        printf("server received packet %d\n", ack);

        if(rand()%100 < ACK_PROB)
        {
            printf("ACK for packet %d lost\n",ack);
        }
        else
        {
            sleep(1);
            printf("ACK send for packet %d\n",ack);
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "%d", ack);
            send(new_socket, buffer, sizeof(buffer), 0);
        }
    }
    close(server_fd);
    close(new_socket);
    return 0;
}