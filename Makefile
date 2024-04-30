CC = gcc
CXX = g++
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
CXXFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c++17 -pedantic
.PHONY:clean

all: server client main

server: server.c file_transfer.c socket_wrapper.c Makefile
	$(CC) $(CFLAGS) server.c file_transfer.c socket_wrapper.c -o server

client: client.c file_transfer.c socket_wrapper.c Makefile
	$(CC) $(CFLAGS) client.c file_transfer.c socket_wrapper.c -o client

main: main.cpp clipboard_handler.cpp Makefile
	$(CXX) $(CXXFLAGS) main.cpp clipboard_handler.cpp -o main

clean:
	rm -f client server main