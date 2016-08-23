ALL = merge-sort
CC = gcc
CFLAGS = -std=c99
CFLAGS += -I../utility
UTIL = ../utility/utility.o
LIBS += -ljansson

OBJs = merge-sort.o

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

merge-sort: $(OBJs)
	$(CC)  -o $@ $(CFLAGS) $(UTIL) $< $(LIBS)

clean:
	rm -f *.o $(ALL)

PHONY: clean
