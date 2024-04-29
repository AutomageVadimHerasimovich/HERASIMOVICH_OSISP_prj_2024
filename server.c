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
    Listen(server, 1);
    socklen_t addrlen = sizeof(adr);
    int fd = Accept(server, &adr, addrlen);
    recieve_file("recieved1.txt",fd);
    recieve_file("recieved2.txt",fd);
    sleep(5);
    close(fd);
    close(server);
}