#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8070
#define BUFFER_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    char filename[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int bytes;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Enter filename: ");
    scanf("%s", filename);

    send(sock, filename, strlen(filename), 0);

    printf("\nFile content:\n");

    while ((bytes = recv(sock, buffer, BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, 1, bytes, stdout);
    }

    close(sock);

    return 0;
}
