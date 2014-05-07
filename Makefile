CC = gcc
LIBS = -lncurses
CFLAGS = -std=gnu99 -ggdb3 -Wall
EXECUTABLE = sudoku

SRC = sudoku.c board.c generator.c
OBJ = $(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all deps clean

all: $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $(OBJ) -o $(EXECUTABLE)

deps:
	$(CC) -MM *.c > Sourcedeps

clean:
	$(RM) $(EXECUTABLE) $(OBJ)
