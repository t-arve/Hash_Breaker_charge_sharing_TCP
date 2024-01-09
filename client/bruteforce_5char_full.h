#ifndef BRUTEFORCE_5CHAR_FULL_H
#define BRUTEFORCE_5CHAR_FULL_H
#define OPENSSL_API_COMPAT 0x10100000L
#include <openssl/md5.h> // Include for MD5 hashing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void checkCollisions(const char *hashesFilePath, const char *outputFilePath);
void generateCombinations(char *prefix, int maxLen, FILE *outputFile, char ***hashes, int *hashCount);
void calculateMD5Hash(char *input, char *output);
int isHashInList(char *hash, char ***hashes, int *hashCount);
void loadHashesFromFile(const char *filePath, char ***hashes, int *hashCount);

// Function implementations
void checkCollisions(const char *hashesFilePath, const char *outputFilePath) {
    char **hashes = NULL; // Array to store loaded hashes
    int hashCount = 0;    // Count of loaded hashes

    // Load hashes from a file
    loadHashesFromFile(hashesFilePath, &hashes, &hashCount);
    printf("Total number of loaded hashes: %d\n", hashCount);

    FILE *outputFile = fopen(outputFilePath, "w");
    if (!outputFile) {
        perror("Unable to open the output file");
        exit(1);
    }

    // Generate combinations of characters to check for collisions
    generateCombinations("", 5, outputFile, &hashes, &hashCount);

    // Free memory
    for (int i = 0; i < hashCount; i++) {
        free(hashes[i]);
    }
    free(hashes);
    fclose(outputFile);
}

void generateCombinations(char *prefix, int maxLen, FILE *outputFile, char ***hashes, int *hashCount) {
    // Check if there are more hashes to find
    if (*hashCount == 0) {
        return;
    }

    char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int charactersLength = sizeof(characters) - 1;
    char newPrefix[6]; // 5 characters + '\0'

    if (maxLen == 0) {
        char hash[MD5_DIGEST_LENGTH * 2 + 1] = {0};
        calculateMD5Hash(prefix, hash);
        if (isHashInList(hash, hashes, hashCount)) {
            fprintf(outputFile, "%s: %s\n", prefix, hash);
            fflush(outputFile); // Force writing to the file
            printf("Collision found: %s: %s\n", prefix, hash);
        }
        return;
    }

    for (int i = 0; i < charactersLength; i++) {
        snprintf(newPrefix, sizeof(newPrefix), "%s%c", prefix, characters[i]);
        generateCombinations(newPrefix, maxLen - 1, outputFile, hashes, hashCount);
    }
}

void calculateMD5Hash(char *input, char *output) {
    unsigned char digest[MD5_DIGEST_LENGTH] = {0};
    MD5((unsigned char *)input, strlen(input), digest);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&output[i * 2], "%02x", (unsigned int)digest[i]);
    }
}

int isHashInList(char *hash, char ***hashes, int *hashCount) {
    // Update: Remove found hashes from the list
    for (int i = 0; i < *hashCount; i++) {
        if (strcmp(hash, (*hashes)[i]) == 0) {
            free((*hashes)[i]);
            for (int j = i; j < (*hashCount) - 1; j++) {
                (*hashes)[j] = (*hashes)[j + 1];
            }
            (*hashCount)--;
            if (*hashCount == 0) {
                return 1;
            }
            // Reallocate to reduce the array size
            *hashes = realloc(*hashes, sizeof(char *) * (*hashCount));
            return 1;
        }
    }
    return 0;
}

void loadHashesFromFile(const char *filePath, char ***hashes, int *hashCount) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Unable to open the hashes file");
        exit(1);
    }

    char line[33] = {0};
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0; // Remove newline characters
        if (strlen(line) > 0) {
            (*hashCount)++;
            *hashes = realloc(*hashes, sizeof(char *) * (*hashCount));
            (*hashes)[*hashCount - 1] = strdup(line);
        }
    }

    fclose(file);
}

#endif // BRUTEFORCE_5CHAR_FULL_H

