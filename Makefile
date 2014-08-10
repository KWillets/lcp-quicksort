CC=gcc
CFLAGS=-I. -O4
DEPS = lcp-quicksort.h
OBJ = lcp-quicksort.o test.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

lcpsort: lcp-quicksort.o lcpsort.o
	gcc -o $@ $^ $(CFLAGS)

genesort: lcp-quicksort.o genesort.o
	gcc -o $@ $^ $(CFLAGS)

clean:  
	rm $(OBJ)