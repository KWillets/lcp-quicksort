CC=gcc
CFLAGS=-I. -O4 -funroll-loops
CPPFLAGS=$(CFLAGS)

DEPS = lcp-quicksort.h
OBJ = lcp-quicksort.o test.o 

%.o: %.c $(DEPS)
	$(CC)  -c -o $@ $< $(CFLAGS)

lcpsort: lcp-quicksort.o lcpsort.o
	g++ -o $@ $^ $(CFLAGS)

lcpsort-sse: lcp-quicksort-sse.o lcpsort.o
	g++ -o $@ $^ $(CFLAGS)

clean:  
	rm *.o