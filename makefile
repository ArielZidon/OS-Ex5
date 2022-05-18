.PHONY: all run clean
CC = gcc
FLAGS= 
HEADERS = 
all: server client runClient

server: server.o
	$(CC) $< -o server -lpthread

client: client.o
	$(CC) $< -o client

ClientTest: clientsTest.o
	$(CC) $< -o clientsTest

runClient: runClient.o ClientTest
	$(CC) $< -o runClient 

	
%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o server client clientsTest runClient