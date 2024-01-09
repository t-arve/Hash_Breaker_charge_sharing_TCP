//server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

void sendData(int sockfd, const char* filename);
void receiveData(int sockfd, const char* filename);


// Function to send data over a socket
void sendData(int sockfd, const char* filename) {
    printf("Sending hashes...\n"); // Debug comment

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
    //printf("Data sent. Closing sending socket.\n");

    // Close the socket for writing, indicating the end of transmission
    shutdown(sockfd, SHUT_WR);

    fclose(file);
}

// Function to receive data from a socket
void receiveData(int sockfd, const char* filename) {
    //printf("Receiving data...\n"); // Debug comment

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("ERROR opening file");
        return;
    }

    char buffer[1024];
    ssize_t n;
    while ((n = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[n] = '\0';
        //printf("Receiving line: %s", buffer); // Debug comment
        fprintf(file, "%s", buffer);
    }

    if (n < 0) {
        perror("ERROR receiving data");
    } else {
        printf("Passwords received.\n"); // Debug comment
    }

    if (n == 0) {
        printf("End of transmission.\n");
    }

    // Ensure data is written to the file
    fflush(file);
    fclose(file);
}





int main(int argc, char *argv[]) {
    // Check for correct command-line arguments
    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        return 1;
    }

    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return 1;
    }

    // Configuration to reuse address and port
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("ERROR in setsockopt");
        close(sockfd);
        return 1;
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    int port = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR in binding");
        return 1;
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept an incoming connection
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("ERROR in accept");
        return 1;
    }
    printf("Client connected.\n");

    FILE *file;
    char fileName[256]; // Ensure the buffer is large enough

    while (1) {
        printf("Select a file to send: ");
        if (fgets(fileName, sizeof(fileName), stdin) == NULL) {
            printf("Error reading file name. Try again.\n");
            continue;
        }
        fileName[strcspn(fileName, "\r\n")] = 0; // Remove the newline at the end

        file = fopen(fileName, "r");
        if (file == NULL) {
            perror("File not found");
            // File not found. The loop repeats, and another file name is requested.
        } else {
            // File found, break the loop.
            fclose(file);
            break;
        }
    }

    // Send the specified file
    sendData(newsockfd, fileName);
    receiveData(newsockfd, "passwords.txt");

    // Close the sockets
    close(newsockfd);
    close(sockfd);
    return 0;
}

