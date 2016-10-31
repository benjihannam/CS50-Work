# makefile for hashtable
#
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBS = -lcurl
PROG = hashtabletest
OBJS = hashtabletest.o hashtable.o ../set/set.o jhash.o

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

hashtabletest.o: hashtable.h ../set/set.h jhash.h
../set/set.o: ../set/set.h
jhash.o: jhash.h


.PHONY: clear

clean:
	rm -f *.o
	rm -f *~
	rm -f hashtabletest

test:
	cd hashtable; ./$(PROG)
