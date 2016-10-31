# makefile for bagtest
#
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBS = -lcurl
PROG = bagtest
OBJS = bagtest.o bag.o

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

bagtest.o: bag.h
bag.o: bag.h           

.PHONY: clear

clean:
	rm -f *.o
	rm -f *~
	rm -f bagtest

test:
	./$(PROG)
	