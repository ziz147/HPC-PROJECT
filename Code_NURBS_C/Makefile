SOURCES=matrix.c surface.c hypersurface.c local_Support_mM.c derTopo.c
OBJECTS=$(SOURCES:.c=.o)

CC=gcc
CFLAGS=-Wall -fopenmp

all: main

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

libsurface.a: $(OBJECTS)
	ar rcs $@ $^

main: test_c.c libsurface.a
	$(CC) $(CFLAGS) $< -o $@ -L. -lsurface -lm

clean:
	rm -f *.o *.a main

text:
	rm -f *.o *.a *.txt main
