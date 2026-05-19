#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define port 8090

void main()
{
    int sockfd;
    struct sockaddr_in address;
    struct timeval tv;
    char buffer[1024],message[1024];
    int bytes,pack;
    int packet,window;

    printf("Enter total no.of packets and window size = ");
    scanf("%d%d",&packet,&window);

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("Socket creation failed...\n");
        exit(1);
    }
    address.sin_family=AF_INET;
    address.sin_port=htons(port);
    inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);

    if(connect(sockfd,(struct sockaddr*)&address,sizeof(address))<0)
    {
        perror("Connection failed.....\n");
        exit(1);
    }

    tv.tv_sec=3;
    tv.tv_usec=0;
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));

    printf("Client connected with the server....\n");

    int base=1;
    int next=1;
    int pckack=0;
    while(pckack<packet)
    {
        while((next<base+window)&&(next<=packet))
        {
            printf("Client:packet %d sent\n",next);
            memset(buffer,0,sizeof(buffer));
            sprintf(buffer,"%d",next);
            send(sockfd,buffer,strlen(buffer)+1,0);
            next++;
        }
        memset(message,0,sizeof(message));
        bytes=read(sockfd,message,sizeof(message));
        
        if(bytes>0)
        {
            pack=atoi(message);
            printf("Client:Ack for packet %d recieved\n",pack);
            if(pack>=base)
            {
                base=pack+1;
                pckack=pack;
            }
        }
        else
        {
            printf("Timeout....packets resending from %d\n",base);
            next=base;
        }
    }
    printf("Client: all packets sent successfully\n");
    close(sockfd);
}