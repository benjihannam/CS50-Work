# makefile for bagtest
#

MAKE = make
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBS = -lcurl

PROG = bag
OBJS = bag.o

# $(PROG): $(OBJS)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

bagtest.o: bag.h
bag.o: bag.h           

.PHONY: clear

clean:
	rm -f *.o
	rm -f *~
	rm -f bag

test:
	cd bag; ./$(PROG) 