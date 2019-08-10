#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

/* Based off of https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/ */

void chat(int sockfd) {
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter Message: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n') {}

        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server: %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit... \n");
            break;
        }
    }
}


int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    
    // Create Client Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket Creation Failed...\n");
        exit(0);
    } else {
        printf("Socket Successfully Created... \n");
    }

    bzero(&servaddr, sizeof(servaddr));

    // Bind Socket to IP Address and Port
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect to Server Port
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed... \n");
        exit(0);
    } else {
        printf("Connected to the server... \n");
    }

    // Run Chat function
    chat(sockfd);

    // Close the socket
    close(sockfd);    


    return 0;
}