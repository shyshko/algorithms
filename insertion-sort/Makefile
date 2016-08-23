ALL = insertion-sort
CC = gcc
CFLAGS = -std=c99
CFLAGS += -I../utility
UTIL = ../utility/utility.o
LIBS += -ljansson

OBJs = insertion-sort.o

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

insertion-sort: $(OBJs)
	$(CC)  -o $@ $(CFLAGS) $(UTIL) $< $(LIBS)

clean:
	rm -f *.o $(ALL)

PHONY: clean
