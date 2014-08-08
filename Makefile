CC=gcc
CFLAGS=-I. -O3
DEPS = lcp-quicksort.h
OBJ = lcp-quicksort.o test.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

lcpsort: lcp-quicksort.o lcpsort.o
	gcc -o $@ $^ $(CFLAGS)
