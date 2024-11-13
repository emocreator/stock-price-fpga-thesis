CC=gcc
CFLAGS=-Wall -Wextra -O2 -g -lm

all: stockpredict visualize

# Stock prediction program
stockpredict: main.o adaboost.o
	$(CC) -o $@ $^ $(CFLAGS)

# Visualization program
visualize: visualization.o adaboost.o
	$(CC) -o $@ $^ $(CFLAGS)

# Object files
%.o: %.c adaboost.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o stockpredict visualize
