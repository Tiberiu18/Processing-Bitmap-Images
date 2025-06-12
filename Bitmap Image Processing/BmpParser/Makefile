CC=gcc
CFLAGS=-Wall -Iinclude
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

BmpParser: $(OBJ)
	$(CC) $(OBJ) -o BmpParser

clean:
	rm -f $(OBJ) BmpParser

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
