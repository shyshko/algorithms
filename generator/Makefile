ALL = generator
CC = gcc
CFLAGS = -std=c99 -g

CFLAGS += -I../utility
UTIL = ../utility/utility.o

OBJs = generator.o

LIBS = -ljansson

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

generator: $(OBJs)
	$(CC) $(LDFLAG) $(CFLAGS)  $(UTIL) $< -o $@ $(LIBS)

clean:
	rm -f *.o $(ALL)

PHONY: clean
