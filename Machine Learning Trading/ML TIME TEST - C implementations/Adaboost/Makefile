CC=gcc
CFLAGS=-Wall -Wextra -O2 -g -lm

all: stockpredict

stockpredict: main.o adaboost.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c adaboost.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o stockpredict
