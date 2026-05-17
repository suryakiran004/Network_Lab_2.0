//probablility srand rand()%100
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>

#define PORT 8080
#define SIZE 1024
#define ACK_PROB 70

int main(){
    int clientSocket,serverSocket;
    struct sockaddr_in clientAddress , serverAddress;
    socklen_t clientLength = sizeof(clientAddress);
    char buffer[SIZE];

     //socket creatiom
    serverSocket = socket(AF_INET,SOCK_STREAM,0);

    //configure the server 
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket,
            (struct sockaddr*)&serverAddress,
            sizeof(serverAddress))<0){
                perror("bind failed.");
                exit(1);
            }
    if(listen(serverSocket,5)<0){
                perror("listen failed.");
                exit(1);
            }
    
    clientSocket = accept(serverSocket,
                        (struct sockaddr*)&clientAddress,
                        &clientLength);
    srand(time(0));
    while(1){
        int bytedRecevied = read(clientSocket,
                                buffer,
                                sizeof(buffer));
        if (bytedRecevied < 0){
            break;
        }
        
        buffer[bytedRecevied]='\0';

        if(strcmp(buffer,"exit")==0){
            printf("[EXIT] : conection closing.\n");
            break;
        }
        printf("[PACKET RECECIVED] : received packet is %s\n",buffer);
        if(rand()%100 < ACK_PROB ){
            send(clientSocket,
                "ACK",
                strlen("ACK"),
                0);
            printf("[ACK] : ack send.\n");
        }else{
            printf("[ACK] : ack lost.\n");
        }
    }
    close(clientSocket);
    close(serverSocket);
    return 0;
}