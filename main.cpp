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
    return 0;
}

//#import <Foundation/Foundation.h>
//
//void clipboardChanged(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo) {
//    NSLog(@"Clipboard contents have changed.");
//}
//
//int main() {
//    CFNotificationCenterAddObserver(CFNotificationCenterGetDistributedCenter(),
//                                    NULL,
//                                    clipboardChanged,
//                                    CFSTR("com.apple.pasteboard.changed"),
//                                    NULL,
//                                    CFNotificationSuspensionBehaviorDeliverImmediately);
//
//    [[NSRunLoop currentRunLoop] run]; // Запускаем цикл обработки событий
//
//    return 0;
//}