# Makefile for 'crawler' module
#
# David Kotz, April 2016
# updated by Xia Zhou, July 2016
# updated further by Benji Hannam, July 2016 to include bag, set and hashtable structures

PROG = crawler
OBJS = crawler.o ../common/web.o ../lib/bag/bag.o ../lib/set/set.o ../lib/hashtable/hashtable.o ../lib/hashtable/jhash.o
LIBS = -lcurl
LLIBS = ../lib/cs50ds.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

# build the crawler
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# crawler source dependencies; add others as needed
crawler.o: ../common/web.h ../lib/memory/memory.h ../lib/bag/bag.h ../lib/set/set.h ../lib/hashtable/hashtable.h ../lib/hashtable/jhash.h
../common/web.o: ../common/web.h
../lib/bag/bag.o: ../lib/bag/bag.h
../lib/set/set.o: ../lib/set/set.h
../hashtable/hashtable.o: ../lib/hashtable/hashtable.h ../lib/hashtable/jhash.h
../lib/hashtable/jhash.o: ../lib/hashtable/jhash.h
# build the library
../lib/cs50ds.a: 
	cd ../lib; $(MAKE)

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
	cd ../lib; $(MAKE) clean
