#include <cstdio>
#include "clipboard_handler.h"

int main() {
    createFile("input.txt");
    FILE *fp = openToReadClipboard();
    FILE *outFile = openToWriteOutputFile("output.txt");
    writeToFile(fp, outFile);
    pclose(fp);
    fclose(outFile);
    printFileContents("output.txt");
    writeToClipboard();
    fp = openToReadClipboard();
    outFile = openToWriteOutputFile("output.txt");
    writeToFile(fp, outFile);
    pclose(fp);
    fclose(outFile);
    printFileContents("output.txt");
    checkChangesInCB();
    return 0;
}
