# makefile for counterstest.c
#
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBS = -lcurl
PROG = counters
OBJS = counters.o

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

counterstest.o: counters.h
counters.o: counters.h             

.PHONY: clear

clean:
	rm -f *.o
	rm -f *~
	rm -f counters

test:
	cd counter; ./$(PROG) 