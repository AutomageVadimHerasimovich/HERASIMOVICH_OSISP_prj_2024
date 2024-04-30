#include "clipboard_handler.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <unistd.h>

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

#include <sys/wait.h>

void runClientProgram() {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Failed to fork\n";
        exit(1);
    }

    if (pid == 0) {
        // We are in the child process
        execl("./client", "client", (char*) nullptr);
        // If execl returns, there was an error
        std::cerr << "Failed to start client\n";
        exit(1);
    } else {
        // We are in the parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child to finish
        if (WIFEXITED(status)) {
            std::cout << "Child exited with status " << WEXITSTATUS(status) << '\n';
        } else {
            std::cout << "Child process did not exit successfully\n";
        }
    }
}

#include <csignal>

pid_t server_pid = -1;

void terminate(int signal) {
    if (server_pid != -1) {
        kill(server_pid, SIGTERM);
    }
    exit(0);
}

pid_t runServerProgram() {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Failed to fork\n";
        exit(1);
    }

    if (pid == 0) {
        // We are in the child process
        execl("./server", "server", (char*) nullptr);
        // If execl returns, there was an error
        std::cerr << "Failed to start server\n";
        exit(1);
    } else {
        // We are in the parent process
        server_pid = pid;
        signal(SIGTERM, terminate);
        int status;
        waitpid(pid, &status, 0); // Wait for the child to finish
        if (WIFEXITED(status)) {
            std::cout << "Child (server) exited with status " << WEXITSTATUS(status) << '\n';
        } else {
            std::cout << "Child (server) process did not exit successfully\n";
        }
    }
    return pid;
}

#include <thread>
void checkChangesInCB() {
    std::string lastClipboardContent = getClipboardContent();
    std::thread serverThred(runServerProgram);
    while (true) {
        sleep(1); /** Check every 1 seconds*/
        std::string currentClipboardContent = getClipboardContent();
        if (currentClipboardContent != lastClipboardContent) {
            std::cout << "Clipboard content has changed.\n";
            lastClipboardContent = currentClipboardContent;
            FILE* fp = openToReadClipboard();
            FILE* outFile = openToWriteOutputFile("output.txt");
            writeToFile(fp, outFile);
            pclose(fp);
            fclose(outFile);
            printFileContents("output.txt");
            std::thread clientThred(runClientProgram);
            clientThred.join();
            //sleep(2);
            //serverThred.join();
        }
    }
}
