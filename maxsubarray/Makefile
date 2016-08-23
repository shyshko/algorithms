ALL = maxsubarray
CC = gcc
CFLAGS = -std=c99
CFLAGS += -I../utility
UTIL = ../utility/utility.o
LIBS += -ljansson

OBJs = maxsubarray.o

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

maxsubarray: $(OBJs)
	$(CC)  -o $@ $(CFLAGS) $(UTIL) $< $(LIBS)

clean:
	rm -f *.o $(ALL)

PHONY: clean
