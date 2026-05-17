#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    struct sockaddr_in server, client;
    int s, n;
    char b1[100], b2[100];

    s=socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family=AF_INET;
    server.sin_port=htons(2000);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    printf("Client ready...\n");
    n=sizeof(server);

    while(1)
    {
        printf("Client: ");
        fgets(b1, sizeof(b1), stdin);
        b1[strcspn(b1, "\n")]='\0';
        sendto(s, b1, sizeof(b1), 0, (struct sockaddr *)&server, n);
        if(!strcmp(b1, "exit"))
        {
            printf("Client exit...\n");
            break;
        }
        recvfrom(s, b2, sizeof(b2), 0, NULL, NULL);
        printf("Server: %s", b2);
    }
    return 0;
}