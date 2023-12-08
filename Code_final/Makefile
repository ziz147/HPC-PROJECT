CC=gcc
CFLAGS=-Wall -Wextra -g

all: main

matrix.o: matrix.c
	$(CC) $(CFLAGS) -c $< -o $@
surface.o: surface.c
	$(CC) $(CFLAGS) -c $< -o $@

hypersurface.o: hypersurface.c
	$(CC) $(CFLAGS) -c $< -o $@

local_Support_mM.o: local_Support_mM.c
	$(CC) $(CFLAGS) -c $< -o $@


libsurface.a: matrix.o surface.o hypersurface.o local_Support_mM.o
	ar rcs $@ $^

main: main.c libsurface.a
	$(CC) $(CFLAGS) $< -o $@ -L. -lsurface

clean:
	rm -f *.o *.a main
text:
	rm -f *.o *.a *.txt main