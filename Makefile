TARGET = server client

CC = gcc
C++ = g++
CFLAGS = -Wall

all:$(TARGET)

server:server.c
	$(CC) $(CFLAGS) -o $@ $<

client:client.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(TARGET)

