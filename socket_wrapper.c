#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket_wrapper.h"

int Socket(int domain, int type, int protocol) {
    int server = socket(domain, type, protocol);
    if (server == -1) {
        perror("fail on create socket\n");
        exit(EXIT_FAILURE);
    }
    return server;
}

void Bind(int socket, struct sockaddr_in *adr, socklen_t socklen) {
    int res = bind(socket, (struct sockaddr *) adr, socklen);
    if (res == -1) {
        perror("Error on bind socket\n");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfg, int backlog) {
    int res = listen(sockfg, backlog);
    if (res == -1) {
        perror("Listen failed\n");
        exit(EXIT_FAILURE);
    }
}

int Accept(int server, struct sockaddr_in *adr, socklen_t socklen) {
    int res = accept(server, (struct sockaddr *) adr, &socklen);
    if (res == -1) {
        perror("Error accept\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Connect(int fd, struct sockaddr_in *adr, socklen_t socklen) {
    int res = connect(fd, (struct sockaddr *) adr, socklen);
    if (res == -1) {
        perror("Error to connect\n");
        exit(EXIT_FAILURE);
    }
}

void Inet_pton(int af, const char *src, void *dst) {
    int res = inet_pton(af, src, dst);
    if (res <= 0) {
        if (res == 0) {
            fprintf(stderr, "inet_pton: not in presentation format\n");
        } else {
            perror("inet_pton");
        }
        exit(EXIT_FAILURE);
    }
}

char* getIPFromMAC(const char* macAddress) {
    // Выполнить команду ping для обновления таблицы ARP
//    char* pingCommand = "ping -c 1 192.168.1.255 > /dev/null";
//    system(pingCommand);
    char* pingCommand = "ping -c 1 192.168.43.255 > /dev/null"; //Universal ip-adress
    system(pingCommand);
    // Затем выполнить arp -a для получения IP-адреса по MAC-адресу
    char arpCommand[128];
    sprintf(arpCommand, "arp -a | grep %s", macAddress);
    char buffer[128];
    FILE* pipe = popen(arpCommand, "r");
    if (!pipe) {
        return strdup("ERROR");
    }
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            break;
    }
    pclose(pipe);

    char* ipStart = strchr(buffer, '(') + 1; // Найти начало IP-адреса
    char* ipEnd = strchr(ipStart, ')'); // Найти конец IP-адреса
    if (ipStart && ipEnd) {
        *ipEnd = '\0'; // Заменить закрывающую скобку на нулевой символ
    }
    return strdup(ipStart);
}

void createServer(int* fd){
    int server = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(53535);
    Bind(server, &adr, sizeof(adr));
    Listen(server, 1);
    socklen_t addrlen = sizeof(adr);
    *fd = Accept(server, &adr, addrlen);
    if (*fd == -1) {
        printf("Failed to accept connection\n");
        close(*fd);
        close(server);
        exit(-1);
    }
}
