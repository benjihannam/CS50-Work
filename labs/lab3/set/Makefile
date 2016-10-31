# makefile for settest.c
#
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBS = -lcurl
PROG = settest
OBJS = settest.o set.o

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

settest.o: set.h  
set.o: set.h          

.PHONY: clear

clean:
	rm -f *.o
	rm -f *~
	rm -f settest

test:
	./$(PROG)
	 