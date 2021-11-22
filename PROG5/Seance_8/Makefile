all: main

CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -Wno-unused-parameter

main: main.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main.o: main.c


clean:
	rm -f main *.o
