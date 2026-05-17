//c (timeval)
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/time.h>
#include<stdlib.h>

#define PORT 8080
#define SIZE 1024
#define TIMEOUT_SECOND 3

int main(){
    int clientSocket;
    struct timeval receivedTimeout;
    struct sockaddr_in serverAddress;
    char packetToSend[SIZE];
    char packetReceived[SIZE];

    //socket creatiom
    clientSocket = socket(AF_INET,SOCK_STREAM,0);

    //configure the server 
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    //configure timeval
    receivedTimeout.tv_sec = TIMEOUT_SECOND;
    receivedTimeout.tv_usec = 0;

    //set socket option 
    setsockopt(clientSocket,
            SOL_SOCKET,
            SO_RCVTIMEO,
            &receivedTimeout,
            sizeof(receivedTimeout));
    
    if(connect(clientSocket,
            (struct sockaddr*)&serverAddress,
            sizeof(serverAddress))<0){
                perror("connection failed");
                exit(1);//exit with failure
            }
    
    while(1){
        printf("Enter packet number to send /'exit' : ");
        scanf("%s",packetToSend);

        if(strcmp(packetToSend,"exit")==0){
            send(clientSocket,
                packetToSend,
                strlen(packetToSend),
                0);
            printf("[EXIT] : conection closing.\n");
            break;
        }

        int ackReceived=0;
        while(!ackReceived){
            send(clientSocket,
                packetToSend,
                strlen(packetToSend),
                0);
            
            int bytedRecevied = read(clientSocket,
                                    packetReceived,
                                    sizeof(packetReceived));
            if(bytedRecevied < 0){
                printf("[TIMEOUT] : ack not received.\n");
                printf("[RETRANSMIT] : retransmitting the packet %s.\n",packetToSend);
                continue;
            }
            packetReceived[bytedRecevied]='\0';
            if(strcmp(packetReceived,"ACK")==0){
                printf("[ACK] : ack received.\n");
                ackReceived = 1;
            }
        }
    }
    close(clientSocket);
    return 0;
}