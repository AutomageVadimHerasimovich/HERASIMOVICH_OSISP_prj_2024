#ifndef CLIPBOARD_HANDLER_H
#define CLIPBOARD_HANDLER_H

#include <cstdio>

FILE* openToReadClipboard();

long getFileSizeInBytes(const char* filename);

/** This function reads the contents of a file and prints them to stdout **/
void printFileContents(const char* filename);

FILE* openToWriteClipboard();

FILE* openToWriteOutputFile(const char* filename);

FILE* openToReadInputFile(const char* filename);

void writeToFile(FILE* fp, FILE* outFile);

void writeToClipboard();

void createFile(const char* filename);

void checkChangesInCB();

void runClientProgram();

#endif // CLIPBOARD_HANDLER_H