.PHONY: all run clean
CC = gcc
FLAGS= 
HEADERS = 
all: server client clientsTest

server: server.o
	$(CC) $< -o server -lpthread

client: client.o
	$(CC) $< -o client

ClientTest: clientsTest.o
	$(CC) $< -o clientsTest
	
%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o server client clientsTest