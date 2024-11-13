CC=gcc
CFLAGS=-Wall -Wextra -O2 -lm

# List the actual source files
SRC=main.c adaboost.c
OBJ=$(SRC:.c=.o)

all: adaboost

adaboost: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c adaboost.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o adaboost
