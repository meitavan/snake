CC=gcc
CLIBS=-lncurses
CFLAGS=-g -Wall


all:
	$(CC) $(CFLAGS) -o game game.c $(CLIBS)

clean:
	rm -rf *.o main
