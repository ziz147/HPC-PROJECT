SOURCES=matrix.c surface.c hypersurface.c local_Support_mM.c derTopo.c

# Liste des fichiers objets correspondants
OBJECTS=$(SOURCES:.c=.o)

all: main

# Règle générale pour compiler les fichiers source en fichiers objets
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