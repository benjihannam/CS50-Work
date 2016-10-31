# Makefile for 'common.a' library
#   
# Benji Hannam, July 2016 

OBJS = file.o index.o web.o webpage.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $@ $^

# add other targets for the other .o files

file.o: file.c file.h

index.o: index.c index.h

web.o: web.c web.h

webpage.o: webpage.c webpage.h ../lib/memory/memory.h

word.o: word.c word.h

# add cleanup commands for other subdirectories
.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB)
	
	