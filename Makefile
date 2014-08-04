CC=gcc
CFLAGS=-I. -g 
DEPS = lcp-quicksort.h
OBJ = lcp-quicksort.o test.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)