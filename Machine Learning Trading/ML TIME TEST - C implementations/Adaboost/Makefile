CC=gcc
CFLAGS=-Wall -Wextra -O2 -lm

all: adaboost visualize

adaboost: main.o adaboost.o
	$(CC) -o $@ $^ $(CFLAGS)

visualize: visualize.o adaboost.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c adaboost.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o adaboost visualize
