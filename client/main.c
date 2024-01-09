//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "bruteforce_5char_full.h"

// Function to send data over a socket
void sendData(int sockfd, const char* filename);

// Function to receive data from a socket
void receiveData(int sockfd, const char* filename);




// Function to send data over a socket
void sendData(int sockfd, const char* filename) {
    printf("Sending data...\n"); // Debug comment

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("ERROR opening file");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        //printf("Sending line: %s", line); // Debug comment
        if (send(sockfd, line, strlen(line), 0) < 0) {
            perror("ERROR sending data");
            break;
        }
    }
    printf("Closing sending socket.\n");
    shutdown(sockfd, SHUT_WR);
    printf("Socket closed for writing.\n");

    fclose(file);
}

// Function to receive data from a socket
void receiveData(int sockfd, const char* filename) {
    printf("Receiving hashes...\n");

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("ERROR opening file for receiving");
        return;
    }

    char buffer[1024];
    ssize_t n;
    while ((n = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[n] = '\0';
        if (fputs(buffer, file) == EOF) {
            perror("ERROR writing to file");
            break;
        }
    }

    if (n < 0) {
        perror("ERROR receiving data");
    } else if (n == 0) {
        printf("End of transmission detected.\n");
    }

    fflush(file); // Ensures data is written to disk
    fclose(file);
    printf("hashes received, and file closed.\n");

    // Calls the checkCollisions function from the header file
    checkCollisions("hashes_received.txt", "hashes_found.txt");
}
int main(int argc, char *argv[]) {
    // Check for correct command-line arguments
    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        return 1;
    }

    int sockfd;
    struct sockaddr_in serv_addr;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    int port = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(port);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR in connection");
        return 1;
    }

    // Receive data from the server
    receiveData(sockfd, "hashes_received.txt");

    // Send the processed data back to the server
    sendData(sockfd, "hashes_found.txt");

    // Close the socket
    close(sockfd);
    return 0;
}



