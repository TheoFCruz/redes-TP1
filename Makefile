CC = gcc
CFLAGS = -Wall -Iinclude

all: exp1 exp2 exp3

create_bin:
	mkdir -p bin

# Experimento 1
exp1: create_bin src/exp1/client.c src/exp1/server.c
	$(CC) $(CFLAGS) src/exp1/client.c -o bin/exp1_client
	$(CC) $(CFLAGS) src/exp1/server.c -o bin/exp1_server

# Experimento 2 
exp2: create_bin src/exp2/client.c src/exp2/server.c
	$(CC) $(CFLAGS) src/exp2/client.c -o bin/exp2_client
	$(CC) $(CFLAGS) src/exp2/server.c -o bin/exp2_server

# Experimento 3
exp3: create_bin src/exp3/client.c src/exp3/server.c
	$(CC) $(CFLAGS) src/exp3/client.c -o bin/exp3_client
	$(CC) $(CFLAGS) src/exp3/server.c -o bin/exp3_server

clean:
	rm -rf bin/*

.PHONY: all clean exp1 exp2 exp3 create_bin
