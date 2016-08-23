ALL = utility.o
CC = gcc
CFLAGS = -std=c99

OBJs = $(ALL)

$(OBJs): $(patsubst %.o, %.c, $(OBJs))
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f  $(ALL)

PHONY: clean
