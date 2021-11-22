all: bitabit

CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -Wno-unused-parameter

bitabit: bitabit.o bit_operations.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

bitabit.o: bitabit.c bit_operations.h
bit_operations.o: bit_operations.c bit_operations.h


clean:
	rm -f bitabit *.o
