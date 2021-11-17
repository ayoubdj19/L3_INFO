all: essai_fap

CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -Wno-unused-parameter

essai_fap: essai_fap.o fap_bug.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

fap_bug.o: fap_bug.c fap.h
essai_fap.o: essai_fap.c fap.h


clean:
	rm -f essai_fap *.o
