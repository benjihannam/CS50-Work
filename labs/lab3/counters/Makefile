# makefile for counterstest.c
#
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBS = -lcurl
PROG = counterstest
OBJS = counterstest.o counters.o

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

counterstest.o: counters.h
counters.o: counters.h             

.PHONY: clear

clean:
	rm -f *.o
	rm -f *~
	rm -f counterstest

test:
	cd counter; ./$(PROG) 