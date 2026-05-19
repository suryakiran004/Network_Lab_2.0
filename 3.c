#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define port 8090

void main()
{
    int sockfd,newsock;
    struct sockaddr_in address;
    socklen_t len=sizeof(address);
    int bytes;
    char buffer[1024];
    int loss=30;

    srand(time(0));
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("Socket creation failed!!\n");
        exit(1);
    }
    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(port);
    if(bind(sockfd,(struct sockaddr*)&address,sizeof(address))<0)
    {
        perror("Bind failed\n");
        exit(1);
    }

    if(listen(sockfd,3)<0)
    {
        perror("Listen failed\n");
        exit(1);
    }
    printf("Server ready for connection...\n");
    if((newsock=accept(sockfd,(struct sockaddr*)&address,&len))<0)
    {
        perror("Client connection failed\n");
        exit(1);
    }
    printf("Server connected....\n");
int expected = 1;

while(1)
{
    memset(buffer,0,sizeof(buffer));
    bytes = read(newsock, buffer, sizeof(buffer));

    if(bytes == 0)
    {
        printf("Client finished sending packets\n");
        break;
    }
    if(bytes < 0)
    {
        perror("Read error");
        break;
    }

    int pkt = atoi(buffer);
    printf("Server: packet %d received\n", pkt);

    if(pkt == expected)
    {
        expected++;
    }
    else
    {
        printf("Out-of-order packet discarded\n");
    }

    int ack = expected - 1;

    if(rand()%100 < loss)
    {
        printf("ACK %d lost\n", ack);
    }
    else
    {
        printf("ACK %d sent\n", ack);
        sprintf(buffer,"%d",ack);
        send(newsock,buffer,strlen(buffer)+1,0);
    }
}
    close(sockfd);
    close(newsock);
}