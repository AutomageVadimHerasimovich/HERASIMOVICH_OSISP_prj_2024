#include "socket_wrapper.h"
#include "file_transfer.h"
#include <netinet/in.h>
#include <unistd.h>


int main(void) {
    int server = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(53535);
    Bind(server, &adr, sizeof(adr));
    while (1) {
    Listen(server, 1);
    socklen_t addrlen = sizeof(adr);
    int fd = Accept(server, &adr, addrlen);
    if (fd == -1) {
        printf("Failed to accept connection\n");
        close(fd);
        close(server);
        exit(-1);
    }
    recieve_file("input.txt", fd);
    }
}
