all: essai_pile

CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -Wno-unused-parameter

essai_pile: essai_pile.o pile.o pile_io.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

pile.o: pile.c pile.h
pile_io.o: pile_io.c pile_io.h pile.h


clean:
	rm -f essai_pile *.o
