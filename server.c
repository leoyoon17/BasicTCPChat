#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

/* Based off of https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/ */

void chat(int sockfd) {
    char buff[MAX];
    int n;

    // Infinite Loop for chat
    for (;;) {
        bzero(buff,MAX);

        // Read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        
        // Print buffer which contains the client's contents
        printf("From client: %s\t To client: ", buff);
        bzero(buff, MAX);
        n = 0;

        // Copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n') {}

        // Send above buffer to client
        write(sockfd, buff, sizeof(buff));

        // If the message contains "exit" then server exit and chat is ended
        if (strncmp("exit", buff, 4) == 0) {
            printf("Exiting server...\n");
            break;
        }
    }
}

int main () {

    // Create Socket
    int sockfd, connfd, len;
    struct sockaddr_in servaddr,cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed... \n");
        exit(0);
    } else {
        printf("Socket successfully created... \n");
    }

    bzero(&servaddr, sizeof(servaddr));

    // Bind IP Address and Port to socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding the newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket binding failed... \n");
        exit(0);
    } else {
        printf("Socket successfully bound... \n");
    }

    // Listening
    if ((listen(sockfd,5)) != 0) {
        printf("Listening failed... \n");
        exit(0);
    } else {
        printf("Server listening... \n");
    }

    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Server accept failed... \n");
        exit(0);
    } else {
        printf("Server accepted the client... \n");
    }

    // Function for chatting between client and server
    chat(connfd);

    // Closing the socketexit(0);
    close(sockfd);



    return 0;
}