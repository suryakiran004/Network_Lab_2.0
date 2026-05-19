#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

#define PORT 8080
#define SIZE 1024
#define TIMEOUT_SECOND 2
#define WINDOW_SIZE 4
#define TOTAL_PACKETS 10 // Total number of packets to send

int main() {
    int clientSocket;
    struct timeval receivedTimeout;
    struct sockaddr_in serverAddress;
    char packetToSend[SIZE];
    char packetReceived[SIZE];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    receivedTimeout.tv_sec = TIMEOUT_SECOND;
    receivedTimeout.tv_usec = 0;

    // Set socket option for read timeout
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &receivedTimeout, sizeof(receivedTimeout));
    
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection failed");
        exit(1);
    }
    
    int base = 0;       // Oldest unacknowledged packet
    int nextSeq = 0;    // Next sequence number to send

    while (base < TOTAL_PACKETS) {
        // Send packets up to the window size limit
        while (nextSeq < base + WINDOW_SIZE && nextSeq < TOTAL_PACKETS) {
            sprintf(packetToSend, "PKT %d", nextSeq);
            send(clientSocket, packetToSend, strlen(packetToSend), 0);
            printf("[SENT] : Packet %d sent.\n", nextSeq);
            nextSeq++;
            
            // Small delay to prevent TCP stream coalescing on localhost, 
            // ensuring the receiver parses distinct strings easily.
            usleep(50000); 
        }

        memset(packetReceived, 0, SIZE);
        
        // Wait for acknowledgments
        int bytesReceived = read(clientSocket, packetReceived, sizeof(packetReceived) - 1);
        
        if (bytesReceived < 0) {
            // Read timeout triggers Go-Back-N
            printf("[TIMEOUT] : ACK missing. GO BACK N -> Retransmitting window from %d.\n", base);
            nextSeq = base; // Reset the sequence pointer back to base
        } else {
            int ackNum;
            // Parse cumulative ACK
            if (sscanf(packetReceived, "ACK %d", &ackNum) == 1) {
                printf("[ACK] : Received ACK %d.\n", ackNum);
                // Slide the window forward if a valid cumulative ACK is received
                if (ackNum >= base) {
                    base = ackNum + 1;
                }
            }
        }
    }

    // Terminate connection
    strcpy(packetToSend, "exit");
    send(clientSocket, packetToSend, strlen(packetToSend), 0);
    printf("[FINISHED] : All packets sent successfully. Connection closing.\n");

    close(clientSocket);
    return 0;
}
