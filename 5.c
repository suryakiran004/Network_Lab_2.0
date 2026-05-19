#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define PORT 8080
#define SIZE 1024
#define ACK_PROB 70 // 70% chance to successfully receive and ACK

int main() {
    int clientSocket, serverSocket;
    struct sockaddr_in clientAddress, serverAddress;
    socklen_t clientLength = sizeof(clientAddress);
    char buffer[SIZE];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed.");
        exit(1);
    }
    
    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed.");
        exit(1);
    }
    
    printf("Server listening on port %d...\n", PORT);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
    srand(time(0));
    
    int expectedSeq = 0; // The next packet sequence number expected

    while (1) {
        memset(buffer, 0, SIZE);
        int bytesReceived = read(clientSocket, buffer, sizeof(buffer) - 1);
        
        if (bytesReceived <= 0) {
            break;
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("[EXIT] : Connection closing.\n");
            break;
        }

        int seqNum;
        // Parse the incoming packet
        if (sscanf(buffer, "PKT %d", &seqNum) == 1) {
            printf("[PACKET RECEIVED] : Received packet %d\n", seqNum);
            
            // Simulate packet loss
            if (rand() % 100 < ACK_PROB) {
                if (seqNum == expectedSeq) {
                    // In-order packet received
                    sprintf(buffer, "ACK %d", expectedSeq);
                    send(clientSocket, buffer, strlen(buffer), 0);
                    printf("[ACK] : Sent ACK %d\n", expectedSeq);
                    expectedSeq++;
                } else {
                    // Out-of-order packet received, drop it and resend last valid ACK
                    printf("[OUT OF ORDER] : Expected %d, got %d. Dropping.\n", expectedSeq, seqNum);
                    if (expectedSeq > 0) {
                        sprintf(buffer, "ACK %d", expectedSeq - 1);
                        send(clientSocket, buffer, strlen(buffer), 0);
                        printf("[ACK] : Sent duplicate ACK %d\n", expectedSeq - 1);
                    }
                }
            } else {
                printf("[LOST] : Simulated loss for packet %d (No ACK sent).\n", seqNum);
            }
        }
    }
    
    close(clientSocket);
    close(serverSocket);
    return 0;
}