#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s, ns, n;
    struct sockaddr_in server, client;
    char b1[100], b2[100];
    socklen_t len;

    // Create TCP socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // Server configuration
    server.sin_family = AF_INET;
    server.sin_port = htons(2000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind socket
    bind(s, (struct sockaddr *)&server, sizeof(server));

    // Listen for clients
    listen(s, 5);

    printf("Server waiting for client...\n");

    len = sizeof(client);

    // Accept client connection
    ns = accept(s, (struct sockaddr *)&client, &len);

    while(1)
    {
        // Receive message from client
        n = recv(ns, b1, sizeof(b1), 0);
        b1[n] = '\0';

        if(!(strcmp(b1, "exit")))
        {
            printf("Client exited\n");
            break;
        }

        printf("Client: %s\n", b1);

        // Server reply
        printf("Server: ");
        fgets(b2, sizeof(b2), stdin);

        // Send reply to client
        send(ns, b2, strlen(b2), 0);
    }

    close(ns);
    close(s);

    return 0;
}