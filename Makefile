TARGET = server client

CC = gcc
CFLAGS = -Wall

all:$(TARGET)

server:server.c
	$(CC) $(CFLAGS) -o $@ $<

client:client.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(TARGET)

