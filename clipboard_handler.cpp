#include "clipboard_handler.h"
#include <cstdlib>
#include <vector>

FILE* openToReadClipboard() {
    FILE *fp = popen("/usr/bin/pbpaste", "r");
    if (fp == nullptr) {
        printf("Failed to run command\n" );
        exit(1);
    }
    return fp;
}

long getFileSizeInBytes(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == nullptr) {
        printf("Failed to open file\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    fclose(file);
    return fileSize;
}
/** This function reads the contents of a file and prints them to stdout **/

void printFileContents(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == nullptr) {
        printf("Failed to open file\n");
        exit(1);
    }

    long fileSize = getFileSizeInBytes(filename);
    std::vector<char> buffer(fileSize);
    unsigned long bytesReadInFile;
    while ((bytesReadInFile = fread(buffer.data(), 1, buffer.size(), file)) > 0) {
        fwrite(buffer.data(), 1, bytesReadInFile, stdout);
    }
    printf("\n\n");
    fclose(file);
}

FILE* openToWriteClipboard() {
    FILE *fp = popen("/usr/bin/pbcopy", "w");
    if (fp == nullptr) {
        printf("Failed to run command\n" );
        exit(1);
    }
    return fp;
}

FILE* openToWriteOutputFile(const char* filename) {
    FILE *outFile = fopen(filename, "w");
    if (outFile == nullptr) {
        printf("Failed to open output file\n");
        exit(1);
    }
    return outFile;
}

FILE* openToReadInputFile(const char* filename) {
    FILE *outFile = fopen(filename, "r");
    if (outFile == nullptr) {
        printf("Failed to open output file\n");
        exit(1);
    }
    return outFile;
}

void writeToFile(FILE* fp, FILE* outFile) {
    char path[1035];
    while (fgets(path, sizeof(path)-1, fp) != nullptr) {
        fprintf(outFile, "%s", path);
    }
}

void writeToClipboard() {
    FILE *toWriteCB = openToWriteClipboard();
    FILE *inputFile = openToReadInputFile("input.txt");
    writeToFile(inputFile, toWriteCB);
    pclose(toWriteCB);
    fclose(inputFile);
}

void createFile(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == nullptr) {
        file = fopen(filename, "w");
        if (file == nullptr) {
            printf("Failed to create file\n");
            exit(1);
        }
    }
    fclose(file);
}

#include <iostream>
#include <unistd.h>

std::string getClipboardContent() {
    char buffer[128];
    std::string result;
    FILE* pipe = popen("/usr/bin/pbpaste", "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

[[noreturn]] void checkChangesInCB() {
    std::string lastClipboardContent = getClipboardContent();
    while (true) {
        sleep(1); /** Check every 5 seconds*/
        std::string currentClipboardContent = getClipboardContent();
        if (currentClipboardContent != lastClipboardContent) {
            std::cout << "Clipboard content has changed.\n";
            lastClipboardContent = currentClipboardContent;
        }
    }
}