CC=gcc -g
CFLAGS=-Wall -lm
OBJS=compliance2.o matrix.o

all: test

test: test.c $(OBJS)
	$(CC)  $^ -o $@  $(CFLAGS)

compliance2.o: compliance2.c compliance2.h
	$(CC) -c $< -o $@ $(CFLAGS)
matrix.o: matrix.c matrix.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o principal
