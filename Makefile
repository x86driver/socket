TARGET = server client daemon

CC = gcc
C++ = g++
CFLAGS = -Wall

all:$(TARGET)

server:server.c
	$(CC) $(CFLAGS) -o $@ $<

client:client.c
	$(CC) $(CFLAGS) -o $@ $<

daemon:daemon.cpp daemon.h
	$(C++) $(CFLAGS) -o $@ $< -lpthread

clean:
	rm -rf $(TARGET)

