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
    
    bind(s, (struct sockaddr *)&server, sizeof(server));

    printf("Server ready, waiting for client...\n");
    n=sizeof(client);

    while(1)
    {
        recvfrom(s, b1, sizeof(b1), 0, (struct sockaddr *)&client, &n);
        if(!strcmp(b1, "exit"))
        {
            printf("Client exit...\n");
            break;
        }
        printf("Client: %s\n", b1);
        printf("Server: ");
        fgets(b2, sizeof(b2), stdin);
        sendto(s, b2, sizeof(b2), 0, (struct sockaddr *)&client, n);
    }
    return(0);
}