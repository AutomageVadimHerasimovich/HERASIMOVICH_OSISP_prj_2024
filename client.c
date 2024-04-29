#include "socket_wrapper.h"
#include "file_transfer.h"
#include <unistd.h>


int main(void) {
    int fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(53535);
    char* macAddress = "38:f9:d3:2f:8c:50"; // Замените на реальный MAC-адрес
    char *ipAddress = getIPFromMAC(macAddress);
    printf("IP address for MAC %s is %s\n", macAddress, ipAddress);
    Inet_pton(AF_INET, ipAddress, &adr.sin_addr);
    Connect(fd, &adr, sizeof(adr));
    send_file("output.txt",fd);
    close(fd);
}
