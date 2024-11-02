CC=c99
CFLAGS=-Wall -Wextra -pedantic -O0 -ggdb
LIBS=-lm
all: main.out predict.out

main.out: utils.c main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

predict.out: utils.c predict.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -f *.o *.out
